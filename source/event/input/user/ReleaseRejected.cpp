#include "event/input/user/ReleaseRejected.hpp"

using namespace fm;
using namespace fm::event::input::user;

ReleaseRejected::ReleaseRejected(const std::string& _message) :
    UserEvent(RELEASE_REJECTED),
    message(_message)
{
}

const std::string& ReleaseRejected::getMessage() const
{
    return message;
}

std::string ReleaseRejected::toString() const
{
    return "RELEASE_REJECTED: " + message;
}
