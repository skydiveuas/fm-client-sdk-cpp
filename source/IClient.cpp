#include "IClient.hpp"

#include "event/input/connection/Received.hpp"

#include "state/IState.hpp"
#include "backend/ClientBackend.hpp"

#include <memory>
#include <fstream>
#include <sstream>

using namespace fm;

IClient::Listener::~Listener()
{
}

IClient::~IClient()
{
}

void IClient::trace(const std::string& message)
{
    listener.trace(message);
}

IClient::IClient(const std::string& coreAddress,
                 const int corePort,
                 const std::string& key,
                 Listener& _listener,
                 boost::asio::io_service& _ioService) :
    IStateMachine([&_listener] (const std::string& msg) { _listener.trace(msg); }, _ioService),
    listener(_listener)
{
    std::unique_ptr<core::CoreClient> core = std::make_unique<core::CoreClient>(
                [&_listener] (const std::string& msg) { _listener.trace(msg); }, coreAddress, corePort, key);
    backend = std::make_unique<backend::ClientBackend>(*this, listener, _ioService, core.release());
}
