#include "Device.hpp"

#include "state/device/Disconnected.hpp"

using namespace fm;
using namespace fm::state;

Device::Device(const std::string& coreAddress,
               const int corePort,
               const std::string& key,
               Listener& listener,
               boost::asio::io_service& ioService) :
    IClient(coreAddress, corePort, key, listener, ioService)
{
    setState(std::make_unique<device::Disconnected>(*this, listener, *backend.get()));
}

std::string Device::toString() const
{
    return "Device";
}
