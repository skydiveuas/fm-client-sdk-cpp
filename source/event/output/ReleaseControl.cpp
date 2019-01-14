#include "event/output/ReleaseControl.hpp"

using namespace fm;
using namespace fm::event::output;

ReleaseControl::ReleaseControl(long _channelId) :
    FacadeEvent(RELEASE_CONTROL),
    channelId(_channelId)
{
}

long ReleaseControl::getChannelId() const
{
    return channelId;
}

std::string ReleaseControl::toString() const
{
    return "RELEASE_CONTROL: channel id: " + std::to_string(channelId);
}
