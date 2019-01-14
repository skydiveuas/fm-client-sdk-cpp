#ifndef DEVICESIMULATOR_HPP
#define DEVICESIMULATOR_HPP

#include "ISimulator.hpp"

#include "Device.hpp"

class DeviceSimulator : public ISimulator
{
public:
    DeviceSimulator(boost::asio::io_service&);

    void start(const std::string&, const int, const std::string&);

private:
    std::unique_ptr<fm::Device> device;

    void handleEvent(const std::shared_ptr<const fm::event::output::FacadeEvent> event) override;
};

#endif // DEVICESIMULATOR_HPP
