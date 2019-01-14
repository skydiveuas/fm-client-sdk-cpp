#include "state/pilot/connected/Recovering.hpp"

#include "event/input/connection/Received.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::pilot;
using namespace fm::state::pilot::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Recovering::Recovering(IState& state) :
    IState(state)
{
}

IState::State Recovering::start()
{
    return nullptr;
}

IState::State Recovering::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Recovering::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

std::string Recovering::toString() const
{
    return "Recovering";
}

IState::State Recovering::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    default:
        return defaultMessageHandle(message);
    }
}
