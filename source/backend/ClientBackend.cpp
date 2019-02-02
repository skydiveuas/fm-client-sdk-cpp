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

    stream = stub->Asynccontrol(&context, &completionQueue, (void*)3);

    void* connectTag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
        std::chrono::seconds(5);

    completionQueue.AsyncNext(&connectTag, &ok, deadline);

    if (ok && connectTag == (void*)3)
    {
        trace("Connection to the facade established");

        readTag = (void*)1;

        toRead = std::make_shared<ControlMessage>();
        stream->Read(toRead.get(), readTag);

        keepReader.store(true);
        ioService.post(std::bind(&ClientBackend::proceedReader, this));
    }
    else
    {
        throw std::runtime_error("Could not connect to the facade");
    }
}


void ClientBackend::closeFacadeConnection()
{
    keepReader.store(false);

    grpc::Status status = grpc::Status::CANCELLED;
    stream->Finish(&status, (void*)4);

    void* releaseTag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
        std::chrono::seconds(5);

    completionQueue.AsyncNext(&releaseTag, &ok, deadline);

    // cleanup reading and sending tasks
    while (releaseTag == (void*)1 || releaseTag == (void*)2)
    {
        completionQueue.AsyncNext(&releaseTag, &ok, deadline);
    }

    if (ok && releaseTag == (void*)4)
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
    stream->Write(message, (void*)2);
}

void ClientBackend::trace(const std::string& message)
{
    listener.trace(message);
}

void ClientBackend::proceedReader()
{
    void* tag;
    bool ok = false;

    auto deadline = std::chrono::system_clock::now() +
        std::chrono::milliseconds(1000);

    // Bartek is it really the best way of handling asynchronous reading?
    // Bartek this will cause posting a lot of 1ms long dummy tasks...
    // Bartek would prefere smth similaral as Java-style onNext callback
    completionQueue.AsyncNext(&tag, &ok, deadline);

    if (ok && tag == readTag)
    {
        using event::input::connection::Received;
        client.notifyEvent(std::make_shared<const Received>(toRead));

        toRead = std::make_shared<ControlMessage>();
        stream->Read(toRead.get(), readTag);
    }

    if (keepReader.load())
    {
        ioService.post(std::bind(&ClientBackend::proceedReader, this));
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
