#ifndef FM_STATE_PILOT_CONNECTED_REQUESTINGCONTROL_HPP
#define FM_STATE_PILOT_CONNECTED_REQUESTINGCONTROL_HPP

#include "state/IState.hpp"

namespace fm
{

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
class RequestingControl : public IState
{
public:
    RequestingControl(IState&, long);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

    long channelId;
};

} // connected

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTED_REQUESTINGCONTROL_HPP
