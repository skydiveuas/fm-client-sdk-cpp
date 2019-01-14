#ifndef FM_STATE_DEVICE_CONNECTED_FLYING_HPP
#define FM_STATE_DEVICE_CONNECTED_FLYING_HPP

#include "state/IState.hpp"

#include "common/channel_management.pb.h"
#include "facade/control/facade_service.pb.h"

#include <vector>

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
class Flying : public IState
{
public:
    Flying(IState&, const std::vector<com::fleetmgr::interfaces::ChannelResponse>&);

    State start() override;

    State handleUserEvent(const event::input::user::UserEvent&) override;

    State handleConnectionEvent(const event::input::connection::ConnectionEvent&) override;

    std::string toString() const override;

private:
    std::vector<com::fleetmgr::interfaces::ChannelResponse> initialChannels;

    State handleMessage(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

    void attachChannels(const std::vector<com::fleetmgr::interfaces::ChannelResponse>&);

    void releaseChannels(const std::vector<long>&);
};

} // connected

} // device

} // state

} // fm

#endif // FM_STATE_DEVICE_CONNECTED_FLYING_HPP
