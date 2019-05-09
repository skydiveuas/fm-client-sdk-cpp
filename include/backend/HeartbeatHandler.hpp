#ifndef FM_BACKEND_HEARTBEATHANDLER_HPP
#define FM_BACKEND_HEARTBEATHANDLER_HPP

#include <boost/log/trivial.hpp>

#include "facade/control/facade_service.pb.h"

namespace fm
{

namespace backend
{

class ClientBackend;

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-07
 * Description:
 */
class HeartbeatHandler
{
public:
    HeartbeatHandler(ClientBackend&);

    void start();

    void end();

    void handleHeartbeat(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

private:
    ClientBackend& backend;

    std::time_t lastReception;

    void onTimeout();

    void log(const boost::log::trivial::severity_level&, const std::string& message);
};

} // backend

} // fm

#endif // FM_BACKEND_HEARTBEATHANDLER_HPP
