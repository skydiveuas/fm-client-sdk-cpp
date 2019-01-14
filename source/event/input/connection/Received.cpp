#include "event/input/connection/Received.hpp"

using namespace fm;
using namespace fm::event::input::connection;

using com::fleetmgr::interfaces::facade::control::ControlMessage;

Received::Received(std::shared_ptr<ControlMessage> _message) :
    ConnectionEvent(RECEIVED),
    message(_message)
{
}

const ControlMessage& Received::getMessage() const
{
    return *(message.get());
}

std::string Received::toString() const
{
    return "RECEIVED:\n" + message->DebugString();
}
