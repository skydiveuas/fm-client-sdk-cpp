#ifndef FM_STATE_PILOT_CONNECTED_OPENINGCHANNELS_HPP
#define FM_STATE_PILOT_CONNECTED_OPENINGCHANNELS_HPP

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
class OpeningChannels : public IState
{
public:
    OpeningChannels(IState&, const std::vector<com::fleetmgr::interfaces::ChannelRequest>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

    std::vector<com::fleetmgr::interfaces::ChannelRequest> channelsToOpen;
};

} // connected

} // pilot

} // state

} // fm

#endif // FM_STATE_PILOT_CONNECTED_OPENINGCHANNELS_HPP
