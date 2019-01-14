#include "state/pilot/connected/OpeningChannels.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/ProcedureRejected.hpp"

#include "state/pilot/connected/Operating.hpp"
#include "state/pilot/connected/ValidatingChannels.hpp"

#include "backend/ClientBackend.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::pilot;
using namespace fm::state::pilot::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

OpeningChannels::OpeningChannels(IState& state, const std::vector<ChannelRequest>& _toOpen) :
    IState(state),
    channelsToOpen(_toOpen)
{
}

IState::State OpeningChannels::start()
{
    ClientMessage message;
    message.set_command(Command::ADD_CHANNELS);
    for (const ChannelRequest& c : channelsToOpen)
    {
        message.mutable_channelsrequest()->add_channels()->CopyFrom(c);
    }
    send(message);
    return nullptr;
}

IState::State OpeningChannels::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State OpeningChannels::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string OpeningChannels::toString() const
{
    return "OpeningChannels";
}

IState::State OpeningChannels::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::ADD_CHANNELS:
        if (message.response() == Response::ACCEPTED)
        {
            std::vector<ChannelResponse> toOpen;
            toOpen.reserve(message.channelsresponse().channels_size());
            for (int i = 0; i < message.channelsresponse().channels_size(); ++i)
            {
                toOpen.push_back(message.channelsresponse().channels(i));
            }
            return std::make_unique<ValidatingChannels>(*this, toOpen);
        }
        else
        {
            listener.onEvent(std::make_shared<ProcedureRejected>(
                                 Command::ADD_CHANNELS, message.message()));
            return std::make_unique<Operating>(*this);
        }

    default:
        return defaultMessageHandle(message);
    }
}
