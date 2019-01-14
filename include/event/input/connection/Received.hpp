#ifndef FM_EVENT_INPUT_CONNECTION_RECEIVED_HPP
#define FM_EVENT_INPUT_CONNECTION_RECEIVED_HPP

#include "event/input/connection/ConnectionEvent.hpp"

#include "facade/control/facade_service.pb.h"

#include <string>

namespace fm
{

namespace event
{

namespace input
{

namespace connection
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-06
 * Description:
 */
class Received : public ConnectionEvent
{
public:
    Received(std::shared_ptr<com::fleetmgr::interfaces::facade::control::ControlMessage>);

    const com::fleetmgr::interfaces::facade::control::ControlMessage&
    getMessage() const;

    std::string toString() const override;

private:
    std::shared_ptr<
    com::fleetmgr::interfaces::facade::control::ControlMessage> message;
};

} // connection

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_CONNECTION_RECEIVED_HPP
