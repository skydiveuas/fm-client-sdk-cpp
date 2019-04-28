#include "Pilot.hpp"

#include "state/pilot/Disconnected.hpp"

#include "backend/ClientBackend.hpp"

#include "core/CoreClient.hpp"

using namespace fm;
using namespace fm::state;

using namespace com::fleetmgr::interfaces;

Pilot::Pilot(boost::asio::io_service& _ioService,
             boost::property_tree::ptree& _configuration,
             Listener& _listener) :
    IClient(_ioService, _configuration, _listener)
{
    setState(std::make_unique<pilot::Disconnected>(*this, _listener, *backend.get()));
}

Pilot::Pilot(boost::asio::io_service& _ioService,
             const std::string& _path,
             Listener& _listener) :
    Pilot(_ioService, *loadConfiguration(_path), _listener)
{
}

std::string Pilot::toString() const
{
    return "Pilot";
}

ListDevicesResponse Pilot::listDevices()
{
    return backend->getCore().listDevices();
}

ListDevicesResponse Pilot::listConnectedDevices()
{
    ListDevicesResponse response = listDevices();
    ListDevicesResponse result;
    for (int i = 0; i < response.devices_size(); ++i)
    {
        const Device& d = response.devices(i);
        if (d.connectionstate() == CONNECTED)
        {
            Device* e = result.mutable_devices()->Add();
            e->CopyFrom(d);
        }
    }
    return result;
}
