#include "state/pilot/Connecting.hpp"

#include "state/pilot/Disconnected.hpp"
#include "state/pilot/Disconnecting.hpp"
#include "state/pilot/Connected.hpp"

#include "event/input/connection/Received.hpp"

#include "event/output/Error.hpp"

#include "backend/ClientBackend.hpp"

#include "core/attach.pb.h"

using namespace fm;
using namespace fm::state;
using namespace fm::state::pilot;

using namespace com::fleetmgr::interfaces;
using namespace com::fleetmgr::interfaces::facade::control;

using namespace event::input::user;
using namespace event::input::connection;
using namespace event::output;

Connecting::Connecting(IState& state, long _deviceId, const std::vector<ChannelRequest>& _channels) :
    IState(state),
    deviceId(_deviceId),
    channels(_channels)
{
}

IState::State Connecting::start()
{
    try
    {
        OperateRequest request;
        request.set_requesteddeviceid(deviceId);
        OperateResponse operateResponse = backend.getCore().operate(request);

        backend.openFacadeConnection(operateResponse);

        ClientMessage message;
        message.set_command(Command::SETUP);
        message.mutable_attach()->set_key(operateResponse.key());
        for (ChannelRequest& c : channels)
        {
            message.mutable_channelsrequest()->add_channels()->CopyFrom(c);
        }
        send(message);

        return nullptr;
    }
    catch(std::exception const& e)
    {
        trace("Operate error: " + std::string(e.what()));
        listener.onEvent(std::make_shared<const FacadeEvent>(FacadeEvent::ERROR));
        return std::make_unique<Disconnected>(*this);
    }
}

IState::State Connecting::handleUserEvent(const UserEvent& event)
{
    return defaultEventHandle(event.toString());
}

IState::State Connecting::handleConnectionEvent(const ConnectionEvent& event)
{
    switch (event.getType())
    {
    case ConnectionEvent::RECEIVED:
        return handleMessage(reinterpret_cast<const Received&>(event).getMessage());

    default:
        return defaultEventHandle(event.toString());
    }
}

IState::State Connecting::handleMessage(const ControlMessage& message)
{
    switch (message.command())
    {
    case Command::SETUP:
        if (message.response() == Response::ACCEPTED)
        {
            std::vector<ChannelResponse> openedChannels;
            openedChannels.reserve(message.channelsresponse().channels_size());
            for (int i = 0; i < message.channelsresponse().channels_size(); ++i)
            {
                openedChannels.push_back(message.channelsresponse().channels(i));
            }
            return std::make_unique<Connected>(*this, openedChannels);
        }
        else
        {
            listener.onEvent(std::make_shared<Error>(message.message()));
            return std::make_unique<Disconnecting>(*this);
        }

    default:
        return defaultMessageHandle(message);
    }
}


std::string Connecting::toString() const
{
    return "Connecting";
}
