#include "core/CoreClient.hpp"

using boost::log::trivial::severity_level;

using namespace fm;
using namespace fm::core;

using namespace com::fleetmgr::interfaces;

using namespace google::protobuf::util;

CoreClient::CoreClient(boost::property_tree::ptree& _configuration, Log _log) :
    log(_log),
    client(_configuration, _log)
{
    options.ignore_unknown_fields = true;
}

AttachResponse CoreClient::attach()
{
    std::string responseString =
            client.execute("/gateway/devices/attach", https::HttpsClient::POST, "");
    log(severity_level::info, "Attach response: " + responseString);
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
    log(severity_level::info, "Operate response (" + body + "): " + responseString);
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
    log(severity_level::info, "List devices response: " + responseString);
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
