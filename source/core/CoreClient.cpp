#include "core/CoreClient.hpp"

using namespace fm;
using namespace fm::core;

using namespace com::fleetmgr::interfaces;

using namespace google::protobuf::util;

CoreClient::CoreClient(const std::string& coreAddress, const int corePort, const std::string& key) :
    client(coreAddress, corePort, key)
{
    options.ignore_unknown_fields = true;
}

AttachResponse CoreClient::attach()
{
    std::string responseString =
            client.execute("/gateway/devices/attach", https::HttpsClient::POST, "");
    std::cout << "Attach response: " << responseString << std::endl;
    AttachResponse response;
    Status status = JsonStringToMessage(responseString, &response, options);
    if (Status::OK == status)
    {
        return response;
    }
    else
    {
        throw std::runtime_error("Could not parse response: " + responseString);
    }
}

OperateResponse CoreClient::operate(const OperateRequest& operateRequest)
{
    std::string body;
    MessageToJsonString(operateRequest, &body);
    std::string responseString =
            client.execute("/gateway/pilots/operate", https::HttpsClient::POST, body);
    std::cout << "Operate response (" << body << "): " << responseString << std::endl;
    OperateResponse response;
    Status status = JsonStringToMessage(responseString, &response, options);
    if (Status::OK == status)
    {
        return response;
    }
    else
    {
        throw std::runtime_error("Could not parse response: " + responseString);
    }
}

ListDevicesResponse CoreClient::listDevices()
{
    std::string responseString =
            client.execute("/devices/", https::HttpsClient::GET, "");
    std::cout << "List devices response: " << responseString << std::endl;
    std::string responseJson = "{ \"devices\" : " + responseString + " }";
    ListDevicesResponse response;
    Status status = JsonStringToMessage(responseJson, &response, options);
    if (Status::OK == status)
    {
        return response;
    }
    else
    {
        throw std::runtime_error("Could not parse response: " + responseJson);
    }
}
