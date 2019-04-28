#include "Device.hpp"

#include "state/device/Disconnected.hpp"

using namespace fm;
using namespace fm::state;

Device::Device(boost::asio::io_service& _ioService,
               boost::property_tree::ptree& _configuration,
               Listener& _listener) :
    IClient(_ioService, _configuration, _listener)
{
    setState(std::make_unique<device::Disconnected>(*this, _listener, *backend.get()));
}

Device::Device(boost::asio::io_service& _ioService,
               const std::string& _path,
               Listener& _listener) :
    Device(_ioService, *loadConfiguration(_path), _listener)
{
}

std::string Device::toString() const
{
    return "Device";
}
