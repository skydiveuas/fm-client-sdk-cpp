#include "PilotSimulator.hpp"

int main(int, char**)
{
    const std::string configPath = "../client-simulator/pilotSim-config.ini";

    std::cout << "Staring Pilot simulation..." << std::endl;

    boost::asio::io_service ioService;

    PilotSimulator simulator(ioService);

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
