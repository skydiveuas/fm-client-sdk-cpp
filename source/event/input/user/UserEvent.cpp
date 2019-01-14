#include "event/input/user/UserEvent.hpp"

using namespace fm;
using namespace fm::event::input::user;

UserEvent::UserEvent(Type _type) :
    IInputEvent(USER_EVENT),
    type(_type)
{
}

UserEvent::~UserEvent()
{
}

UserEvent::Type UserEvent::getType() const
{
    return type;
}

std::string UserEvent::toString() const
{
    switch (type)
    {
    case ATTACH: return "ATTACH";
    case RELEASE: return "RELEASE";
    case OPERATE: return "OPERATE";
    case REQUEST_CONTROL: return "REQUEST_CONTROL";
    case RELEASE_ACCEPTED: return "RELEASE_ACCEPTED";
    case RELEASE_REJECTED: return "RELEASE_REJECTED";
    case CONTROL_READY: return "CONTROL_READY";
    case OPEN_CHANNELS: return "OPEN_CHANNELS";
    case CLOSE_CHANNELS: return "CLOSE_CHANNELS";

    default:
        return "Unexpected UserEvent::Type";
    }
}
