#include "backend/ClientBackend.hpp"

#include "IClient.hpp"

#include "core/CoreClient.hpp"
#include "backend/HeartbeatHandler.hpp"
#include "backend/ChannelsHandler.hpp"

#include "event/input/connection/Received.hpp"

#include "traffic/ChannelImpl.hpp"

#include "common/channel_management.pb.h"

#include <fstream>
#include <sstream>

using boost::log::trivial::severity_level;

using namespace fm;
using namespace fm::backend;
using namespace fm::traffic::socket;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

ClientBackend::ClientBackend(IClient& _client,
                             IClient::Listener& _listener,
                             boost::asio::io_service& _ioService,
                             boost::property_tree::ptree& _configuration,
                             core::CoreClient* _core) :
    client(_client),
    listener(_listener),
    ioService(_ioService),
    configuration(_configuration),
    core(_core),
    heartbeatHandler(*this),
    channelsHandler(*this),
    channel(nullptr),
    stub(nullptr),
    stream(nullptr),
    keepReader(false)
{
}

core::CoreClient& ClientBackend::getCore()
{
    return *core;
}

HeartbeatHandler& ClientBackend::getHeartbeatHandler()
{
    return heartbeatHandler;
}

ChannelsHandler& ClientBackend::getChannelsHandler()
{
    return channelsHandler;
}

boost::asio::io_service& ClientBackend::getIoService()
{
    return ioService;
}

boost::property_tree::ptree& ClientBackend::getConfiguration()
{
    return configuration;
}

std::unique_ptr<Location> ClientBackend::getLocation()
{
    return listener.getLocation();
}

void ClientBackend::openFacadeConnection(const com::fleetmgr::interfaces::AttachResponse& attachResponse)
{
    openFacadeConnection(
                attachResponse.host(),
                attachResponse.unsafeport(),
                attachResponse.tlsport(),
                configuration.get<bool>("facade.useTls"));
}

void ClientBackend::openFacadeConnection(const com::fleetmgr::interfaces::OperateResponse& operateResponse)
{
    openFacadeConnection(
                operateResponse.host(),
                operateResponse.unsafeport(),
                operateResponse.tlsport(),
                configuration.get<bool>("facade.useTls"));
}

void ClientBackend::closeFacadeConnection()
{
    log(severity_level::info, "Closing facade connection");

    keepReader.store(false);

    grpc::Status status = grpc::Status::CANCELLED;
    stream->Finish(&status, reinterpret_cast<void*>(CLOSE));

    void* releaseTag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
            std::chrono::seconds(5);

    completionQueue.AsyncNext(&releaseTag, &ok, deadline);

    // cleanup reading and writing tasks
    while (releaseTag == reinterpret_cast<void*>(READ) ||
           releaseTag == reinterpret_cast<void*>(WRITE))
    {
        completionQueue.AsyncNext(&releaseTag, &ok, deadline);
    }

    if (ok && releaseTag == reinterpret_cast<void*>(CLOSE))
    {
        log(severity_level::info, "Connection to the facede closed with: " +
              (status.ok() ? "success" : status.error_message()));
    }
    else
    {
        throw std::runtime_error("Could not close facade connection");
    }
}

void ClientBackend::send(const ClientMessage& message)
{
    severity_level level = severity_level::info;
    if (message.command() == Command::HEARTBEAT)
    {
        // for heartbeats set lower trace level, there will be a lot of these messages
        level = severity_level::debug;
    }
    log(level, "Sending:\n" + message.DebugString() + " @ " + client.getStateName());

    std::lock_guard<std::mutex> sendingLockGuard(sendingLock);
    if (sending.load())
    {
        sendingQueue.push_back(message);
    }
    else
    {
        sending.store(true);
        stream->Write(message, reinterpret_cast<void*>(WRITE));
    }
}

void ClientBackend::log(const severity_level& level, const std::string& message)
{
    listener.log(level, message);
}

void ClientBackend::openFacadeConnection(const std::string& host, const int unsafePort, const int tlsPort, const bool useTls)
{
    if (useTls)
    {
        std::string address = host + ":" + std::to_string(tlsPort);
        log(severity_level::info, "Opening TLS facade channel at: " + address);

        std::string cert;
        readCert(configuration.get<std::string>("cert.facadeCertPath"), cert);

        grpc::SslCredentialsOptions sslOpts;
        sslOpts.pem_root_certs = cert;

        grpc::ChannelArguments args;
        args.SetSslTargetNameOverride("localhost");

        std::shared_ptr<grpc::ChannelCredentials> creds = grpc::SslCredentials(sslOpts);

        channel = grpc::CreateCustomChannel(address, creds, args);
    }
    else
    {
        std::string address = host + ":" + std::to_string(unsafePort);
        log(severity_level::info, "Opening Unsafe facade channel at: " + address);

        channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    }

    stub = FacadeService::NewStub(channel);
    stream = stub->Asynccontrol(&context, &completionQueue, reinterpret_cast<void*>(OPEN));

    void* connectTag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
            std::chrono::seconds(5);

    completionQueue.AsyncNext(&connectTag, &ok, deadline);

    if (ok && connectTag == reinterpret_cast<void*>(OPEN))
    {
        log(severity_level::info, "Connection to the facade established");

        toRead = std::make_shared<ControlMessage>();
        stream->Read(toRead.get(), reinterpret_cast<void*>(READ));

        sending.store(false);
        keepReader.store(true);
        ioService.post(std::bind(&ClientBackend::proceedGrpcQueue, this));
    }
    else
    {
        throw std::runtime_error("Could not connect to the facade");
    }
}

void ClientBackend::proceedGrpcQueue()
{
    void* tag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
            std::chrono::milliseconds(1);

    // Bartek is it really the best way of handling asynchronous reading?
    // Bartek this will cause posting a lot dummy tasks if nothing is ongoing...
    // Bartek would prefer smth similaral as Java-style onNext callback
    completionQueue.AsyncNext(&tag, &ok, deadline);

    if (ok)
    {
        switch (static_cast<GrpcTag>(reinterpret_cast<long>(tag)))
        {
        case READ:
            using event::input::connection::Received;
            client.notifyEvent(std::make_shared<const Received>(toRead));
            toRead = std::make_shared<ControlMessage>();
            stream->Read(toRead.get(), reinterpret_cast<void*>(READ));
            break;

        case WRITE:
        {
            std::lock_guard<std::mutex> sendingLockGuard(sendingLock);
            if (sendingQueue.empty())
            {
                sending.store(false);
            }
            else
            {
                stream->Write(sendingQueue.front(), reinterpret_cast<void*>(WRITE));
                sendingQueue.pop_front();
            }
            break;
        }
        }
    }

    if (keepReader.load())
    {
        ioService.post(std::bind(&ClientBackend::proceedGrpcQueue, this));
    }
}

void ClientBackend::readCert(const std::string& filename, std::string& data)
{
    std::ifstream file (filename.c_str(), std::ios::in);
    if (file.is_open())
    {
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        data = ss.str();
    }
    else
    {
        log(severity_level::info, "Could not open cert file! Path: " + filename);
    }
}
