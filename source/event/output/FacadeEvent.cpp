#include "event/output/FacadeEvent.hpp"

using namespace fm;
using namespace fm::event::output;

FacadeEvent::FacadeEvent(Type _type) :
    type(_type)
{
}

FacadeEvent::~FacadeEvent()
{
}

FacadeEvent::Type FacadeEvent::getType() const
{
    return type;
}

std::string FacadeEvent::toString() const
{
    switch (type)
    {
    case ERROR: return "ERROR";
    case ATTACHED: return "ATTACHED";
    case RELEASED: return "RELEASED";
    case OPERATION_STARTED: return "OPERATION_STARTED";
    case OPERATION_UPDATED: return "OPERATION_UPDATED";
    case CHANNELS_OPENED: return "CHANNELS_OPENED";
    case HANDOVER_ACCEPTED: return "HANDOVER_ACCEPTED";
    case REQUEST_CONTROL: return "REQUEST_CONTROL";
    case RELEASE_CONTROL: return "RELEASE_CONTROL";
    case PROCEDURE_REJECTED: return "PROCEDURE_REJECTED";
    case HANDOVER_DONE: return "HANDOVER_DONE";
    case CONTROL_RELEASED: return "CONTROL_RELEASED";
    case OPERATION_ENDED: return "OPERATION_ENDED";
    case CONNECTION_LOST: return "CONNECTION_LOST";
    case CONNECTION_RECOVERED: return "CONNECTION_RECOVERED";

    default:
        return "Unexpected UserEvent::Type";
    }
}
