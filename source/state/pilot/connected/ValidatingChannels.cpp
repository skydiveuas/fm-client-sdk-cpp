#include "state/pilot/connected/ValidatingChannels.hpp"

#include "state/pilot/connected/Operating.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/ChannelsOpened.hpp"
#include "event/output/Error.hpp"

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

ValidatingChannels::ValidatingChannels(IState& state, const std::vector<ChannelResponse>& openedChannels) :
    IState(state),
    toValidate(openedChannels)
{
}

IState::State ValidatingChannels::start()
{
    validated = backend.getChannelsHandler().validateChannels(toValidate);
    ClientMessage response;
    response.set_command(Command::CHANNELS_READY);
    for (traffic::IChannel* c : validated)
    {
        response.mutable_channelsindication()->add_ids(c->getId());
    }
    send(response);
    return nullptr;
}

IState::State ValidatingChannels::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State ValidatingChannels::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

std::string ValidatingChannels::toString() const
{
    return "ValidatingChannels";
}

IState::State ValidatingChannels::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::CHANNELS_READY:
        if (message.response() == Response::ACCEPTED)
        {
            std::vector<long> owned;
            owned.reserve(message.channelsindication().ids_size());
            for (int i = 0; i < message.channelsindication().ids_size(); ++i)
            {
                owned.push_back(message.channelsindication().ids(i));
            }
            backend.getChannelsHandler().setOwned(owned);
            listener.onEvent(std::make_shared<ChannelsOpened>(validated));
            return std::make_unique<Operating>(*this);
        }
        else
        {
            listener.onEvent(std::make_shared<Error>(message.message()));
            return defaultMessageHandle(message);
        }

    default:
        return defaultMessageHandle(message);
    }
}
