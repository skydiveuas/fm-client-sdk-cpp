#include "event/input/user/CloseChannels.hpp"

#include <sstream>
#include <iterator>

using namespace fm;
using namespace fm::event::input::user;

CloseChannels::CloseChannels(const std::vector<long>& _channels) :
    UserEvent(CLOSE_CHANNELS),
    channels(_channels)
{
}

const std::vector<long>& CloseChannels::getChannels() const
{
    return channels;
}

std::string CloseChannels::toString() const
{
    std::ostringstream oss;
    oss << "[";
    if (!channels.empty())
    {
        std::copy(channels.begin(), channels.end()-1,
                  std::ostream_iterator<long>(oss, ","));
        oss << channels.back();
    }
    oss << "]";
    return "CLOSE_CHANNELS: channels: " + oss.str();
}
