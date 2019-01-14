#include "event/output/ProcedureRejected.hpp"

using namespace fm;
using namespace fm::event::output;

using namespace com::fleetmgr::interfaces::facade::control;

ProcedureRejected::ProcedureRejected(Command _procedure, const std::string& _message) :
    FacadeEvent(PROCEDURE_REJECTED),
    procedure(_procedure),
    message(_message)
{
}

Command ProcedureRejected::getCommand() const
{
    return procedure;
}

const std::string& ProcedureRejected::getMessage() const
{
    return message;
}

std::string ProcedureRejected::toString() const
{
    return "PROCEDURE_REJECTED: " + Command_Name(procedure) + ": " + message;
}
