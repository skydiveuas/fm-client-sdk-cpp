#ifndef FM_STATE_PILOT_CONNECTED_CLOSINGCHANNELS_HPP
#define FM_STATE_PILOT_CONNECTED_CLOSINGCHANNELS_HPP

#include "state/IState.hpp"

#include <vector>

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
 * Date: 2018-12-27
 * Description:
 */
class ClosingChannels : public IState
{
public:
    ClosingChannels(IState&, const std::vector<long>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

    std::vector<long> channelsToClose;
};

} // connected

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTED_CLOSINGCHANNELS_HPP
