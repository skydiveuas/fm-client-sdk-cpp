#ifndef FM_CORE_CORECLIENT_HPP
#define FM_CORE_CORECLIENT_HPP

#include "https/HttpsClient.hpp"

#include "core/attach.pb.h"
#include "core/operate.pb.h"
#include "core/list_devices.pb.h"

#include "google/protobuf/util/json_util.h"

namespace fm
{

namespace core
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-29
 * Description:
 */
class CoreClient
{
public:
    CoreClient(const std::string&, const int, const std::string&);

    com::fleetmgr::interfaces::AttachResponse attach();

    com::fleetmgr::interfaces::OperateResponse operate(const com::fleetmgr::interfaces::OperateRequest&);

    com::fleetmgr::interfaces::ListDevicesResponse listDevices();

private:
   https::HttpsClient client;

   google::protobuf::util::JsonParseOptions options;
};

} // core

} // fm

#endif // FM_CORE_CORECLIENT_HPP
