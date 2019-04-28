#include "IClient.hpp"

#include "event/input/connection/Received.hpp"

#include "state/IState.hpp"
#include "backend/ClientBackend.hpp"

#include <boost/property_tree/ini_parser.hpp>

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

IClient::IClient(boost::asio::io_service& _ioService,
                 boost::property_tree::ptree& _configuration,
                 Listener& _listener) :
    IStateMachine([&_listener] (const std::string& msg) { _listener.trace(msg); }, _ioService),
    listener(_listener)
{
    std::unique_ptr<core::CoreClient> core = std::make_unique<core::CoreClient>(
                _configuration,
                [&_listener] (const std::string& msg) { _listener.trace(msg); });
    backend = std::make_unique<backend::ClientBackend>(*this, listener, _ioService, _configuration, core.release());
}

std::unique_ptr<boost::property_tree::ptree> IClient::loadConfiguration(const std::string& path) {
    std::unique_ptr<boost::property_tree::ptree> pt = std::make_unique<boost::property_tree::ptree>();
    boost::property_tree::ini_parser::read_ini(path, *pt);
    return pt;
}
