#include "backend/HeartbeatHandler.hpp"

#include "backend/ClientBackend.hpp"

using boost::log::trivial::severity_level;

using namespace fm;
using namespace fm::backend;

using namespace com::fleetmgr::interfaces::facade::control;


HeartbeatHandler::HeartbeatHandler(ClientBackend& _backend) :
    backend(_backend)
{
}

void HeartbeatHandler::start()
{

}

void HeartbeatHandler::end()
{

}

void HeartbeatHandler::handleHeartbeat(const ControlMessage& message)
{
    lastReception = std::time(0);
    ClientMessage response;
    response.set_command(HEARTBEAT);
    response.set_response(ACCEPTED);
    response.mutable_heartbeat()->set_key(message.heartbeat().key());
    response.mutable_heartbeat()->set_allocated_location(backend.getLocation().release());
    backend.send(response);
}

void HeartbeatHandler::onTimeout()
{

}

void HeartbeatHandler::log(const severity_level& level, const std::string& message)
{
    backend.log(level, message);
}
