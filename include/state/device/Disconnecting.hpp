#ifndef FM_STATE_DEVICE_DISCONNECTING_HPP
#define FM_STATE_DEVICE_DISCONNECTING_HPP

#include "state/IState.hpp"

namespace fm
{

namespace state
{

namespace device
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class Disconnecting: public IState
{
public:
    Disconnecting(IState&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;
};

} // device

} // state

} // fm

#endif // FM_STATE_DEVICE_DISCONNECTING_HPP
