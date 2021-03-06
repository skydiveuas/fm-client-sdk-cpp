#include "PilotSimulator.hpp"

#include "core/list_devices.pb.h"

#include "event/input/user/Operate.hpp"
#include "event/input/user/OpenChannels.hpp"
#include "event/input/user/CloseChannels.hpp"
#include "event/input/user/RequestControl.hpp"

#include "event/output/ProcedureRejected.hpp"

using boost::log::trivial::severity_level;

using namespace fm;
using namespace fm::event;

using namespace com::fleetmgr::interfaces;

using namespace fm::event::input::user;
using namespace fm::event::output;

using namespace com::fleetmgr::interfaces::facade::control;

PilotSimulator::PilotSimulator(boost::asio::io_service& ioService) :
    ISimulator(ioService),
    pilot(nullptr)
{
}

void PilotSimulator::start(const std::string& configPath)
{
    pilot = std::make_unique<fm::Pilot>(ioService, configPath, *this);
    ListDevicesResponse response = pilot->listConnectedDevices();
    if (response.devices_size() > 0)
    {
        long deviceId = response.devices(0).id();
        std::vector<ChannelRequest> channels;
        ChannelRequest channelsReq;
        channelsReq.set_id(1);
        channelsReq.set_protocol(Protocol::UDP);
        channelsReq.set_security(Security::PLAIN_TEXT);
        channelsReq.set_priority(Priority::NEAR_REAL_TIME);
        channels.push_back(channelsReq);
        channelsReq.set_id(8);
        channelsReq.set_protocol(Protocol::UDP);
        channelsReq.set_security(Security::PLAIN_TEXT);
        channels.push_back(channelsReq);
        std::shared_ptr<const Operate> o = std::make_shared<const Operate>(deviceId, channels);
        ioService.post([this, o] ()
        {
            pilot->notifyEvent(o);
        });
    }
    else
    {
        throw std::runtime_error("No device to connect");
    }
}

void PilotSimulator::handleEvent(const std::shared_ptr<const FacadeEvent> event)
{
    switch (event->getType())
    {
    case FacadeEvent::CHANNELS_OPENED:
        //emmitEvent(std::make_shared<UserEvent>(UserEvent::RELEASE), 10);
        emmitEvent(std::make_shared<CloseChannels>(std::vector<long>{8}), 5);
        break;

    case FacadeEvent::CHANNELS_CLOSING:
        emmitEvent(std::make_shared<RequestControl>(1), 5);
        break;

    case FacadeEvent::HANDOVER_ACCEPTED:
        emmitEvent(std::make_shared<UserEvent>(UserEvent::CONTROL_READY), 0);
        break;

    case FacadeEvent::HANDOVER_DONE:
        break;

    case FacadeEvent::PROCEDURE_REJECTED:
    {
        const ProcedureRejected& rejected = reinterpret_cast<const ProcedureRejected&>(*event);
        if (rejected.getCommand() == Command::REQUEST_CONTROL)
        {
            log(severity_level::info, "HO request rejected: " + rejected.getMessage());
            emmitEvent(std::make_shared<UserEvent>(UserEvent::RELEASE), 5);
        }
        break;
    }

    case FacadeEvent::ERROR:
    case FacadeEvent::RELEASED:
        done.store(true);
        break;
    }
}

void PilotSimulator::emmitEvent(std::shared_ptr<const UserEvent> event, long waiting)
{
    std::thread t([this, event, waiting] ()
    {
        std::this_thread::sleep_for(std::chrono::seconds(waiting));
        ioService.post([this, event] ()
        {
            pilot->notifyEvent(event);
        });
    });
    t.detach();
}
