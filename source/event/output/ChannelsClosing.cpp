#include "event/output/ChannelsClosing.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::output;

ChannelsClosing::ChannelsClosing(const std::vector<traffic::IChannel*>& _channels) :
    FacadeEvent(CHANNELS_CLOSING),
    channels(_channels)
{
}

const std::vector<traffic::IChannel*>& ChannelsClosing::getChannels() const
{
    return channels;
}

std::string ChannelsClosing::toString() const
{
    std::ostringstream oss;
    oss << "[";
    for (auto c : channels)
    {
        oss << c->getId() << ",";
    }
    oss << "]";
    return "CHANNELS_CLOSING: " + oss.str();
}
