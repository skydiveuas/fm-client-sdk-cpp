#include "event/input/connection/ConnectionEvent.hpp"

using namespace fm;
using namespace fm::event::input::connection;

ConnectionEvent::ConnectionEvent(Type _type) :
    IInputEvent(CONNECTION_EVENT),
    type(_type)
{
}

ConnectionEvent::~ConnectionEvent()
{
}

ConnectionEvent::Type ConnectionEvent::getType() const
{
    return type;
}

std::string ConnectionEvent::toString() const
{
    switch (type)
    {
    case CLOSED: return "CLOSED";
    case ERROR: return "ERROR";
    case RECEIVED: return "RECEIVED";
    case LOST: return "LOST";

    default:
        return "Unexpected ConnectionEvent::Type";
    }
}
