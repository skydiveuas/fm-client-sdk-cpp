#ifndef FM_STATE_DEVICE_CONNECTED_RELEASED_HPP
#define FM_STATE_DEVICE_CONNECTED_RELEASED_HPP

#include "state/IState.hpp"

namespace fm
{

namespace state
{

namespace device
{

namespace connected
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-09
 * Description:
 */
class Released : public IState
{
public:
    Released(IState&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    State createOuterState() override;

    std::string toString() const override;

private:
    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);
};

} // connected

} // device

} // state

} // fm

#endif // FM_STATE_DEVICE_CONNECTED_RELEASED_HPP
