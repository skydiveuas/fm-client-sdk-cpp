#include "event/input/user/RequestControl.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::input::user;

RequestControl::RequestControl(long _channelId) :
    UserEvent(REQUEST_CONTROL),
    channelId(_channelId)
{
}

long RequestControl::getChannelId() const
{
    return channelId;
}

std::string RequestControl::toString() const
{
    return "REQUEST_CONTROL: channel id " + std::to_string(channelId);
}
