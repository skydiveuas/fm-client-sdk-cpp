#include "state/pilot/connected/Released.hpp"

#include "state/pilot/Disconnecting.hpp"

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

Released::Released(IState& state) :
    IState(state)
{
}

IState::State Released::start()
{
    return nullptr;
}

IState::State Released::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Released::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Released::createOuterState()
{
    return std::make_unique<Disconnecting>(*this);
}

std::string Released::toString() const
{
    return "Released";
}

IState::State Released::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    default:
        return defaultMessageHandle(message);
    }
}
