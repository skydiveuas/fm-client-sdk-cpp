#ifndef FM_CORE_CORECLIENT_HPP
#define FM_CORE_CORECLIENT_HPP

#include "https/HttpsClient.hpp"

#include "core/attach.pb.h"
#include "core/operate.pb.h"
#include "core/list_devices.pb.h"

#include "google/protobuf/util/json_util.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>

#include <functional>

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
    typedef std::function<void(const boost::log::trivial::severity_level&, const std::string&)> Log;

    CoreClient(boost::property_tree::ptree&, Log);

    com::fleetmgr::interfaces::AttachResponse attach();

    com::fleetmgr::interfaces::OperateResponse operate(const com::fleetmgr::interfaces::OperateRequest&);

    com::fleetmgr::interfaces::ListDevicesResponse listDevices();

private:
   Log log;

   https::HttpsClient client;

   google::protobuf::util::JsonParseOptions options;
};

} // core

} // fm

#endif // FM_CORE_CORECLIENT_HPP
