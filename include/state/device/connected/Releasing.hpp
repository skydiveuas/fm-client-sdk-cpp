#ifndef FM_STATE_DEVICE_CONNECTED_RELEASEING_HPP
#define FM_STATE_DEVICE_CONNECTED_RELEASEING_HPP

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
class Releasing : public IState
{
public:
    Releasing(IState&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);
};

} // connected

} // device

} // state

} // fm

#endif // FM_STATE_DEVICE_CONNECTED_RELEASEING_HPP
