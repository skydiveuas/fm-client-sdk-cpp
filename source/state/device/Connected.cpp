#include "state/device/Connected.hpp"

#include "state/device/connected/Ready.hpp"

#include "backend/ClientBackend.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::device;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Connected::Connected(IState& state) :
    IState(state),
    internalState(std::make_unique<connected::Ready>(*this))
{
}

IState::State Connected::start()
{
    backend.getHeartbeatHandler().start();
    internalState->start();
    listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::ATTACHED));
    return nullptr;
}

IState::State Connected::handleUserEvent(const UserEvent& event)
{
    return onNewState(reinterpret_cast<IState*>(internalState.get())->handleUserEvent(event));
}

IState::State Connected::handleConnectionEvent(const ConnectionEvent& event)
{
    return onNewState(reinterpret_cast<IState*>(internalState.get())->handleConnectionEvent(event));
}

std::string Connected::toString() const
{
    return "Connected::" + internalState->toString();
}

IState::State Connected::onNewState(State newState)
{
    while (nullptr != newState.get())
    {
        listener.trace("Connected transition: " + internalState->toString() + " -> " + newState->toString());
        internalState.swap(newState);
        newState.reset(internalState->start().release());
    }
    return reinterpret_cast<IState*>(internalState.get())->createOuterState();
}
