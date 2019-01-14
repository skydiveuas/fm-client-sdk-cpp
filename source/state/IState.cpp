#include "state/IState.hpp"

#include "IClient.hpp"

#include "backend/ClientBackend.hpp"

using namespace fm;
using namespace fm::state;

using namespace com::fleetmgr::interfaces::facade::control;

using event::input::user::UserEvent;
using event::input::connection::ConnectionEvent;

IState::~IState()
{
}
IState::State IState::handleEvent(const std::shared_ptr<const event::input::IInputEvent> event)
{
    switch (event->getInputEventType())
    {
    case event::input::IInputEvent::USER_EVENT:
        return handleUserEvent(reinterpret_cast<const UserEvent&>(*(event.get())));

    case event::input::IInputEvent::CONNECTION_EVENT:
        return handleConnectionEvent(reinterpret_cast<const ConnectionEvent&>(*(event.get())));
    }
}

IState::IState(IState& state) :
    client(state.client),
    listener(state.listener),
    backend(state.backend)
{
}

IState::IState(IClient& _client, IClient::Listener& _listener, backend::ClientBackend& _backend) :
    client(_client),
    listener(_listener),
    backend(_backend)
{
}

IState::State IState::handleUserEvent(const UserEvent& event)
{
    return defaultEventHandle(event.toString());
}

IState::State IState::handleConnectionEvent(const ConnectionEvent& event)
{
    return defaultEventHandle(event.toString());
}

IState::State IState::createOuterState()
{
    return nullptr;
}

void IState::send(const ClientMessage& message)
{
    backend.send(message);
}

IState::State IState::defaultEventHandle(const std::string& eventName)
{
    trace("Unexpected: " + eventName + " @ " + toString());
    return nullptr;
}

IState::State IState::defaultMessageHandle(const ControlMessage& message)
{
    if (message.command() == Command::HEARTBEAT && message.has_heartbeat())
    {
        backend.getHeartbeatHandler().handleHeartbeat(message);
    }
    else
    {
        trace("Unexpected ControlMessage received:\n" + message.DebugString() + " @ " + toString());
    }
    return nullptr;
}

void IState::trace(const std::string& message)
{
    listener.trace(message);
}
