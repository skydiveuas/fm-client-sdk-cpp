#include "DeviceSimulator.hpp"

int main(int, char**)
{
    const std::string apiKey = "ApiKey ft0aiBWIiLTZspHjc3n8oL07lhsGh3FrGoFYLhVHjJs208uN0xoWZESNQRO7x8XuP+CsAP0fxx5H6Z3zB9J2O/fUFuXeL0ymjseAc/PmFvV8YUVEzZM7bdpOUGw5PXfU";

//    const std::string host = "edge.zhjixouahfpdviebd.com";
//    const int port = 443;

    const std::string host = "192.168.1.69";
    const int port = 14010;

    std::cout << "Staring Device simulation..." << std::endl;

    boost::asio::io_service ioService;

    DeviceSimulator simulator(ioService);
    simulator.start(host, port, apiKey);

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
