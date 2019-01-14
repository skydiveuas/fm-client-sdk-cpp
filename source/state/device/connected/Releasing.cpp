#include "state/device/connected/Releasing.hpp"

#include "state/device/connected/Released.hpp"

#include "event/input/connection/Received.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::device;
using namespace fm::state::device::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Releasing::Releasing(IState& state) :
    IState(state)
{
}

IState::State Releasing::start()
{
    ClientMessage message;
    message.set_command(RELEASE);
    send(message);
    return nullptr;
}

std::string Releasing::toString() const
{
    return "Releasing";
}

IState::State Releasing::handleUserEvent(const UserEvent& event)
{
    return defaultEventHandle(event.toString());
}

IState::State Releasing::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Releasing::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case RELEASE:
        if (ACCEPTED == message.response())
        {
            return std::make_unique<Released>(*this);
        }
        else
        {
            return defaultMessageHandle(message);
        }

    default:
        return defaultMessageHandle(message);
    }
}
