#include "PilotSimulator.hpp"

int main(int, char**)
{
    const std::string apiKey = "ApiKey at+ya8GFZpVrCAKHKMi4DhiP5YS14uk6OR+NHR85Tdf7ocxdO4BoMSmoqAEjVexyoeh1hzNfpcrZX1aQOQa/ijCj0gY+F5//QQOJZRGWQUFLUP3WlkyVFzKhD26Faane";

    const std::string host = "192.168.1.69";
    const int port = 60010;

    std::cout << "Staring Pilot simulation..." << std::endl;

    boost::asio::io_service ioService;

    PilotSimulator simulator(ioService);

    try
    {
        simulator.start(host, port, apiKey);
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
