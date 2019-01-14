#include "event/input/user/ReleaseAccepted.hpp"

using namespace fm;
using namespace fm::event::input::user;

ReleaseAccepted::ReleaseAccepted(const std::vector<uint8_t>& _data) :
    UserEvent(RELEASE_ACCEPTED),
    data(_data)
{
}

const std::vector<uint8_t>& ReleaseAccepted::getData() const
{
    return data;
}

std::string ReleaseAccepted::toString() const
{
    return "RELEASE_ACCEPTED: data size: " + std::to_string(data.size());
}
