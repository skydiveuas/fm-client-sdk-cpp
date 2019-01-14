#include "state/pilot/connected/ReleasingControl.hpp"

#include "state/pilot/connected/Operating.hpp"

#include "event/output/ReleaseControl.hpp"

#include "event/input/user/ReleaseAccepted.hpp"
#include "event/input/user/ReleaseRejected.hpp"

#include "event/input/connection/Received.hpp"

#include "backend/ClientBackend.hpp"
#include "backend/ChannelsHandler.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::pilot;
using namespace fm::state::pilot::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

ReleasingControl::ReleasingControl(IState& state, long _channelId) :
    IState(state),
    channelId(_channelId)
{
}

IState::State ReleasingControl::start()
{
    listener.onEvent(std::make_shared<ReleaseControl>(channelId));
    return nullptr;
}

IState::State ReleasingControl::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    case UserEvent::RELEASE_ACCEPTED:
    {
        const ReleaseAccepted releaseEvent = reinterpret_cast<const ReleaseAccepted&>(event);
        ClientMessage clientMessage;
        clientMessage.set_command(Command::RELEASE_CONTROL);
        clientMessage.set_response(Response::ACCEPTED);
        clientMessage.mutable_handoverdata()->set_handoverdata(
                    std::string(releaseEvent.getData().begin(), releaseEvent.getData().end()));
        send(clientMessage);
        return nullptr;
    }

    case UserEvent::RELEASE_REJECTED:
    {
        const ReleaseRejected releaseEvent = reinterpret_cast<const ReleaseRejected&>(event);
        ClientMessage clientMessage;
        clientMessage.set_command(Command::RELEASE_CONTROL);
        clientMessage.set_response(Response::ACCEPTED);
        clientMessage.set_message(releaseEvent.getMessage());
        send(clientMessage);
        return std::make_unique<Operating>(*this);
    }

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State ReleasingControl::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string ReleasingControl::toString() const
{
    return "ReleasingControl";
}

IState::State ReleasingControl::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::CONTROL_RELEASED:
    {
        backend.getChannelsHandler().setOwned(channelId, false);
        listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::CONTROL_RELEASED));
        ClientMessage clientMessage;
        clientMessage.set_command(Command::CONTROL_RELEASED);
        clientMessage.set_response(Response::ACCEPTED);
        send(clientMessage);
        return std::make_unique<Operating>(*this);
    }

    default:
        return defaultMessageHandle(message);
    }
}
