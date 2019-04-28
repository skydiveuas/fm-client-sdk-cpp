#include "DeviceSimulator.hpp"

int main(int, char**)
{
    const std::string configPath = "../client-simulator/deviceSim-config.ini";

    std::cout << "Staring Device simulation..." << std::endl;

    boost::asio::io_service ioService;

    DeviceSimulator simulator(ioService);

    try
    {
        simulator.start(configPath);
    }
    catch (std::exception& e)
    {
        std::cout << "Could not start simulator: " << e.what() << std::endl;
        return -1;
    }

    while (not simulator.isDone())
    {
        try
        {
            ioService.run();
        }
        catch (const std::exception& e)
        {
            std::cout << "Task execution error: " << e.what() << std::endl;
        }
    }

    std::cout << " >>>>>>> DONE <<<<<<<" << std::endl;
    return 0;
}
