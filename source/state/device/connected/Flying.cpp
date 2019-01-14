#include "state/device/connected/Flying.hpp"

#include "state/device/connected/Ready.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/ChannelsOpened.hpp"
#include "event/output/ChannelsClosing.hpp"

#include "backend/ClientBackend.hpp"

using namespace fm;
using namespace fm::state;
using namespace fm::state::device;
using namespace fm::state::device::connected;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Flying::Flying(IState& state, const std::vector<ChannelResponse>& _initialChannels) :
    IState(state),
    initialChannels(_initialChannels)
{
}

IState::State Flying::start()
{
    attachChannels(initialChannels);
    return nullptr;
}

std::string Flying::toString() const
{
    return "Flying";
}

IState::State Flying::handleUserEvent(const UserEvent& event)
{
    switch (event.getType())
    {
    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Flying::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Flying::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::ATTACH_CHANNELS:
    {
        std::vector<ChannelResponse> openedeChannels;
        openedeChannels.reserve(message.channelsresponse().channels_size());
        for (int i = 0; i < message.channelsresponse().channels_size(); ++i)
        {
            openedeChannels.emplace_back(message.channelsresponse().channels(i));
        }
        attachChannels(openedeChannels);
        return nullptr;
    }

    case Command::RELEASE_CHANNELS:
    {
        std::vector<long> releasedChannels;
        releasedChannels.reserve(message.channelsindication().ids_size());
        for (int i = 0; i < message.channelsindication().ids_size(); ++i)
        {
            releasedChannels.emplace_back(message.channelsindication().ids(i));
        }
        releaseChannels(releasedChannels);
        return nullptr;
    }

    case Command::OPERATION_ENDED:
    {
        listener.onEvent(std::make_shared<ChannelsClosing>(
                             backend.getChannelsHandler().getChannels()));
        backend.getChannelsHandler().closeAllChannels();
        listener.onEvent(std::make_shared<FacadeEvent>(FacadeEvent::OPERATION_ENDED));
        ClientMessage response;
        response.set_command(Command::OPERATION_ENDED);
        response.set_response(Response::ACCEPTED);
        send(response);
        return std::make_unique<Ready>(*this);
    }

    default:
        return defaultMessageHandle(message);
    }
}

void Flying::attachChannels(const std::vector<ChannelResponse>& channels)
{
    std::vector<traffic::IChannel*> result =
            backend.getChannelsHandler().validateChannels(channels);
    listener.onEvent(std::make_shared<ChannelsOpened>(result));
    ClientMessage response;
    response.set_command(Command::ATTACH_CHANNELS);
    response.set_response(Response::ACCEPTED);
    for (traffic::IChannel* c : result)
    {
        response.mutable_channelsindication()->add_ids(c->getId());
    }
    send(response);
}

void Flying::releaseChannels(const std::vector<long>& channels)
{
    listener.onEvent(std::make_shared<ChannelsClosing>(
                         backend.getChannelsHandler().getChannels(channels)));
    backend.getChannelsHandler().closeChannels(channels);
    ClientMessage response;
    response.set_command(Command::RELEASE_CHANNELS);
    response.set_response(Response::ACCEPTED);
    for (long id : channels)
    {
        response.mutable_channelsindication()->add_ids(id);
    }
    send(response);
}
