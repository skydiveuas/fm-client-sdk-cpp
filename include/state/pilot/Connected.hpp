#ifndef FM_STATE_PILOT_CONNECTED_HPP
#define FM_STATE_PILOT_CONNECTED_HPP

#include "state/IState.hpp"

#include "common/role.pb.h"
#include "common/channel_management.pb.h"

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
class Connected : public IState
{
public:
    Connected(IState&, const std::vector<com::fleetmgr::interfaces::ChannelResponse>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    State internalState;

    State onNewState(State);
};

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTED_HPP
