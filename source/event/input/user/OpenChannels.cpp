#include "event/input/user/OpenChannels.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::input::user;

using namespace com::fleetmgr::interfaces;

OpenChannels::OpenChannels(const std::vector<ChannelRequest>& _channels) :
    UserEvent(OPEN_CHANNELS),
    channels(_channels)
{
}

const std::vector<ChannelRequest>& OpenChannels::getChannels() const
{
    return channels;
}

std::string OpenChannels::toString() const
{
    std::ostringstream oss;
    oss << "[";
    for (const ChannelRequest& c : channels)
    {
        oss << c.id() << ",";
    }
    oss << "]";
    return "OPEN_CHANNELS: channels: " + oss.str();
}
