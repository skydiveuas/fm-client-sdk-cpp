#include "event/output/Error.hpp"

using namespace fm;
using namespace fm::event::output;

Error::Error(const std::string& _message) :
    FacadeEvent(ERROR),
    message(_message)
{
}

const std::string& Error::getMessage() const
{
    return message;
}

std::string Error::toString() const
{
    return "ERROR: " + message;
}
