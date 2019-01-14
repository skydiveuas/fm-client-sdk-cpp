#ifndef FM_STATE_PILOT_CONNECTING_HPP
#define FM_STATE_PILOT_CONNECTING_HPP

#include "state/IState.hpp"

#include "common/role.pb.h"

#include <vector>

namespace fm
{

namespace state
{

namespace pilot
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class Connecting : public IState
{
public:
    Connecting(IState&, long, const std::vector<com::fleetmgr::interfaces::ChannelRequest>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    long deviceId;
    std::vector<com::fleetmgr::interfaces::ChannelRequest> channels;

    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);
};

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTING_HPP
