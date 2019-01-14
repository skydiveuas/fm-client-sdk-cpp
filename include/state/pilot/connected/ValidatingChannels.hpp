#ifndef FM_STATE_PILOT_CONNECTED_VALIDATINGCHANNELS_HPP
#define FM_STATE_PILOT_CONNECTED_VALIDATINGCHANNELS_HPP

#include "state/IState.hpp"

#include "common/role.pb.h"
#include "common/channel_management.pb.h"

#include <vector>

namespace fm
{

namespace traffic {
class IChannel;
}

namespace state
{

namespace pilot
{

namespace connected
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-09
 * Description:
 */
class ValidatingChannels : public IState
{
public:
    ValidatingChannels(IState&, const std::vector<com::fleetmgr::interfaces::ChannelResponse>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    std::vector<com::fleetmgr::interfaces::ChannelResponse> toValidate;
    std::vector<traffic::IChannel*> validated;

    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);
};

} // connected

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTED_RELEASEING_HPP
