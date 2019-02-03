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

using namespace fm;
using namespace fm::backend;
using namespace fm::traffic::socket;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

ClientBackend::ClientBackend(IClient& _client, IClient::Listener& _listener, boost::asio::io_service& _ioService, core::CoreClient* _core) :
    client(_client),
    listener(_listener),
    ioService(_ioService),
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

std::unique_ptr<Location> ClientBackend::getLocation()
{
    return listener.getLocation();
}

void ClientBackend::openFacadeConnection(const std::string& host, const int port)
{
    trace("Opening facade channel at: " + host + ":" + std::to_string(port));

    std::string address = host + ":" + std::to_string(port);

    std::string cert;
    readCert("grpc_facade.crt", cert);

    grpc::SslCredentialsOptions sslOpts;
    sslOpts.pem_root_certs = cert;

    grpc::ChannelArguments args;
    args.SetSslTargetNameOverride("localhost");

    std::shared_ptr<grpc::ChannelCredentials> creds = grpc::SslCredentials(sslOpts);

    channel = grpc::CreateCustomChannel(address, creds, args);
    stub = FacadeService::NewStub(channel);

    stream = stub->Asynccontrol(&context, &completionQueue, reinterpret_cast<void*>(OPEN));

    void* connectTag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
            std::chrono::seconds(5);

    completionQueue.AsyncNext(&connectTag, &ok, deadline);

    if (ok && connectTag == reinterpret_cast<void*>(OPEN))
    {
        trace("Connection to the facade established");

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

void ClientBackend::closeFacadeConnection()
{
    trace("Closing facade connection");

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
        trace("Connection to the facede closed with: " +
              (status.ok() ? "success" : status.error_message()));
    }
    else
    {
        throw std::runtime_error("Could not close facade connection");
    }
}

void ClientBackend::send(const ClientMessage& message)
{
    trace("Sending:\n" + message.DebugString() + " @ " + client.getStateName());
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

void ClientBackend::trace(const std::string& message)
{
    listener.trace(message);
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
}
