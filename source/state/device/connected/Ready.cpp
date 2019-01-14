#include "state/device/connected/Ready.hpp"

#include "state/device/connected/Releasing.hpp"
#include "state/device/connected/Flying.hpp"

#include "event/input/connection/Received.hpp"

#include "core/attach.pb.h"

using namespace fm;
using namespace fm::state;
using namespace fm::state::device;
using namespace fm::state::device::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Ready::Ready(IState& state) :
    IState(state)
{
}

IState::State Ready::start()
{
    return nullptr;
}

IState::State Ready::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    case UserEvent::RELEASE:
        return std::make_unique<Releasing>(*this);

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Ready::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string Ready::toString() const
{
    return "Ready";
}

IState::State Ready::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::ATTACH_CHANNELS:
    {
        std::vector<ChannelResponse> openedChannels;
        openedChannels.reserve(message.channelsresponse().channels_size());
        for (int i = 0; i < message.channelsresponse().channels_size(); ++i)
        {
            openedChannels.emplace_back(message.channelsresponse().channels(i));
        }
        return std::make_unique<Flying>(*this, openedChannels);
    }

    default:
        return defaultMessageHandle(message);
    }
}
