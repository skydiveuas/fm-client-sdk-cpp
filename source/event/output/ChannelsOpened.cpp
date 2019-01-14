#include "event/output/ChannelsOpened.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::output;

ChannelsOpened::ChannelsOpened(const std::vector<traffic::IChannel*>& _channels) :
    FacadeEvent(CHANNELS_OPENED),
    channels(_channels)
{
}

const std::vector<traffic::IChannel*>& ChannelsOpened::getChannels() const
{
    return channels;
}

std::string ChannelsOpened::toString() const
{
    std::ostringstream oss;
    oss << "[";
    for (auto c : channels)
    {
        oss << c->getId() << ",";
    }
    oss << "]";
    return "CHANNELS_OPENED: " + oss.str();
}
