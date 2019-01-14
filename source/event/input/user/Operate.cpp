#include "event/input/user/Operate.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::input::user;

using namespace com::fleetmgr::interfaces;

Operate::Operate(long _deviceId, const std::vector<ChannelRequest>& _channels) :
    UserEvent(OPERATE),
    deviceId(_deviceId),
    channels(_channels)
{
}

long Operate::getDeviceId() const
{
    return deviceId;
}

const std::vector<ChannelRequest>& Operate::getChannels() const
{
    return channels;
}

std::string Operate::toString() const
{
    std::ostringstream oss;
    for (const ChannelRequest& c : channels)
    {
        oss << c.id() << ",";
    }
    return "OPERATE: " + std::to_string(deviceId) + " channels: " + oss.str();
}
