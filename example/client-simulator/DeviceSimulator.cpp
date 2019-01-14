#include "DeviceSimulator.hpp"

#include "event/input/user/UserEvent.hpp"

using namespace fm;
using namespace fm::event;

using namespace com::fleetmgr::interfaces;

using fm::event::input::user::UserEvent;
using fm::event::output::FacadeEvent;

DeviceSimulator::DeviceSimulator(boost::asio::io_service& ioService) :
    ISimulator(ioService),
    device(nullptr)
{
}

void DeviceSimulator::start(const std::string& coreAddress,
                            const int corePort,
                            const std::string& key)
{
    device = std::make_unique<fm::Device>(coreAddress, corePort, key, *this, ioService);
    ioService.post([this] ()
    {
        device->notifyEvent(std::make_shared<const UserEvent>(UserEvent::ATTACH));
    });
}

void DeviceSimulator::handleEvent(const std::shared_ptr<const FacadeEvent> event)
{
    switch (event->getType())
    {
    case FacadeEvent::OPERATION_ENDED:
        ioService.post([this] ()
        {
            device->notifyEvent(std::make_shared<UserEvent>(UserEvent::RELEASE));
        });
        break;

    case FacadeEvent::ERROR:
    case FacadeEvent::RELEASED:
        done.store(true);
        break;
    }
}
