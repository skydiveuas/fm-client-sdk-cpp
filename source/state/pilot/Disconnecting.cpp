#include "state/pilot/Disconnecting.hpp"

#include "state/pilot/Disconnected.hpp"

#include "backend/ClientBackend.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::pilot;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Disconnecting::Disconnecting(IState& state) :
    IState(state)
{
}

IState::State Disconnecting::start()
{
    backend.getHeartbeatHandler().end();
    backend.closeFacadeConnection();
    listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::RELEASED));
    return std::make_unique<Disconnected>(*this);
}

IState::State Disconnecting::handleUserEvent(const UserEvent& event)
{
    return defaultEventHandle(event.toString());
}

IState::State Disconnecting::handleConnectionEvent(const ConnectionEvent& event)
{
    return defaultEventHandle(event.toString());
}

std::string Disconnecting::toString() const
{
    return "Disconnecting";
}
