#ifndef FM_EVENT_OUTPUT_PROCEDUREREJECTED_HPP
#define FM_EVENT_OUTPUT_PROCEDUREREJECTED_HPP

#include "event/output/FacadeEvent.hpp"

#include "facade/control/facade_service.pb.h"

namespace fm
{

namespace event
{

namespace output
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-27
 * Description:
 */
class ProcedureRejected : public FacadeEvent
{
public:
    ProcedureRejected(com::fleetmgr::interfaces::facade::control::Command, const std::string&);

    com::fleetmgr::interfaces::facade::control::Command getCommand() const;

    const std::string& getMessage() const;

    std::string toString() const;

private:
    com::fleetmgr::interfaces::facade::control::Command procedure;
    std:: string message;
};

} // output

} // event

} // fm

#endif // FM_EVENT_OUTPUT_PROCEDUREREJECTED_HPP
