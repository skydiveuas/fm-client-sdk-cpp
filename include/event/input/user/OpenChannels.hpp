#ifndef FM_EVENT_INPUT_USER_OPENCHANNELS_HPP
#define FM_EVENT_INPUT_USER_OPENCHANNELS_HPP

#include "event/input/user/UserEvent.hpp"

#include "common/channel_management.pb.h"

#include <vector>

namespace fm
{

namespace event
{

namespace input
{

namespace user
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-09
 * Description:
 */
class OpenChannels : public UserEvent
{
public:
    OpenChannels(const std::vector<com::fleetmgr::interfaces::ChannelRequest>&);

    const std::vector<com::fleetmgr::interfaces::ChannelRequest>& getChannels() const;

    std::string toString() const override;

private:
    std::vector<com::fleetmgr::interfaces::ChannelRequest> channels;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_OPENCHANNELS_HPP
