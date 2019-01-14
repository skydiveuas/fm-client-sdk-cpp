#include "state/pilot/connected/ClosingChannels.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/ChannelsClosing.hpp"
#include "event/output/ProcedureRejected.hpp"

#include "state/pilot/connected/Operating.hpp"
#include "state/pilot/connected/Released.hpp"

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

ClosingChannels::ClosingChannels(IState& state, const std::vector<long>& _toClose) :
    IState(state),
    channelsToClose(_toClose)
{
}

IState::State ClosingChannels::start()
{
    ClientMessage message;
    message.set_command(Command::REMOVE_CHANNELS);
    for (long c : channelsToClose)
    {
        message.mutable_channelsindication()->add_ids(c);
    }
    send(message);
    return nullptr;
}

std::string ClosingChannels::toString() const
{
    return "ClosingChannels";
}

IState::State ClosingChannels::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State ClosingChannels::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State ClosingChannels::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::REMOVE_CHANNELS:
        if (message.response() == Response::ACCEPTED)
        {
            listener.onEvent(std::make_shared<ChannelsClosing>(
                                 backend.getChannelsHandler().getChannels(channelsToClose)));
            backend.getChannelsHandler().closeChannels(channelsToClose);
            ClientMessage message;
            message.set_command(Command::REMOVE_CHANNELS);
            message.set_response(Response::ACCEPTED);
            send(message);
            return std::make_unique<Operating>(*this);
        }
        else
        {
            listener.onEvent(std::make_shared<ProcedureRejected>(
                                 Command::REMOVE_CHANNELS, message.message()));
            return std::make_unique<Operating>(*this);
        }

    case Command::RELEASE:
    {
        listener.onEvent(std::make_shared<ChannelsClosing>(
                             backend.getChannelsHandler().getChannels()));
        backend.getChannelsHandler().closeAllChannels();
        ClientMessage message;
        message.set_command(Command::RELEASE);
        message.set_response(Response::ACCEPTED);
        send(message);
        return std::make_unique<Released>(*this);
    }

    default:
        return defaultMessageHandle(message);
    }
}
