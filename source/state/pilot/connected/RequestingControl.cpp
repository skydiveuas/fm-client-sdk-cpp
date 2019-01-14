#include "state/pilot/connected/RequestingControl.hpp"

#include "state/pilot/connected/Operating.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/HandoverAccepted.hpp"
#include "event/output/ProcedureRejected.hpp"

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

RequestingControl::RequestingControl(IState& state, long _channelId) :
    IState(state),
    channelId(_channelId)
{
}

IState::State RequestingControl::start()
{
    ClientMessage clientMessage;
    clientMessage.set_command(Command::REQUEST_CONTROL);
    clientMessage.mutable_channelsindication()->add_ids(channelId);
    send(clientMessage);
    return nullptr;
}

IState::State RequestingControl::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    case UserEvent::CONTROL_READY:
    {
        ClientMessage clientMessage;
        clientMessage.set_command(Command::CONTROL_READY);
        send(clientMessage);
        return nullptr;
    }

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State RequestingControl::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string RequestingControl::toString() const
{
    return "RequestingControl";
}

IState::State RequestingControl::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::REQUEST_CONTROL:
        if (message.response() == Response::ACCEPTED)
        {
            listener.onEvent(std::make_shared<HandoverAccepted>(message.handoverdata().handoverdata()));
            return nullptr;
        }
        else
        {
            listener.onEvent(std::make_shared<ProcedureRejected>(message.command(), message.message()));
            return std::make_unique<Operating>(*this);
        }

    case Command::CONTROL_READY:
        if (message.response() == Response::ACCEPTED)
        {
            backend.getChannelsHandler().setOwned(channelId, true);
            listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::HANDOVER_DONE));
            return std::make_unique<Operating>(*this);
        }
        else
        {
            return defaultMessageHandle(message);
        }

    default:
        return defaultMessageHandle(message);
    }
}
