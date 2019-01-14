#include "state/pilot/connected/Operating.hpp"

#include "event/input/connection/Received.hpp"
#include "event/input/user/OpenChannels.hpp"
#include "event/input/user/CloseChannels.hpp"
#include "event/input/user/RequestControl.hpp"

#include "event/output/ChannelsClosing.hpp"

#include "state/pilot/connected/OpeningChannels.hpp"
#include "state/pilot/connected/ClosingChannels.hpp"
#include "state/pilot/connected/RequestingControl.hpp"
#include "state/pilot/connected/ReleasingControl.hpp"
#include "state/pilot/connected/Recovering.hpp"
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

Operating::Operating(IState& state) :
    IState(state)
{
}

IState::State Operating::start()
{
    client.recall();
    return nullptr;
}

IState::State Operating::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    case UserEvent::OPEN_CHANNELS:
    {
        const OpenChannels& openChannels = reinterpret_cast<const OpenChannels&>(event);
        return std::make_unique<OpeningChannels>(*this, openChannels.getChannels());
    }

    case UserEvent::CLOSE_CHANNELS:
    {
        const CloseChannels& closeChannels = reinterpret_cast<const CloseChannels&>(event);
        return std::make_unique<ClosingChannels>(*this, closeChannels.getChannels());
    }

    case UserEvent::REQUEST_CONTROL:
    {
        const RequestControl& requestControl = reinterpret_cast<const RequestControl&>(event);
        return std::make_unique<RequestingControl>(*this, requestControl.getChannelId());
    }

    case UserEvent::RELEASE:
        return std::make_unique<ClosingChannels>(*this, backend.getChannelsHandler().getChannelsIds());

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Operating::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    case ConnectionEvent::LOST:
        return std::make_unique<Recovering>(*this);

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string Operating::toString() const
{
    return "Operating";
}

IState::State Operating::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::OPERATION_UPDATED:
        listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::OPERATION_UPDATED));
        client.recall();
        return nullptr;

    case Command::RELEASE_CONTROL:
        return std::make_unique<ReleasingControl>(*this, message.channelsindication().ids(0));

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
