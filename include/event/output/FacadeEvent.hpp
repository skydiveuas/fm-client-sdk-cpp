#ifndef FM_EVENT_OUTPUT_FACADEEVENT_HPP
#define FM_EVENT_OUTPUT_FACADEEVENT_HPP

#include <string>

namespace fm
{

namespace event
{

namespace output
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class FacadeEvent
{
public:
    enum Type
    {
        ERROR,
        ATTACHED,
        RELEASED,
        OPERATION_STARTED,
        OPERATION_UPDATED,
        CHANNELS_OPENED,
        HANDOVER_ACCEPTED,
        REQUEST_CONTROL,
        RELEASE_CONTROL,
        PROCEDURE_REJECTED,
        HANDOVER_DONE,
        CONTROL_RELEASED,
        CHANNELS_CLOSING,
        OPERATION_ENDED,
        CONNECTION_LOST,
        CONNECTION_RECOVERED,
    };

    FacadeEvent(Type);

    virtual ~FacadeEvent();

    Type getType() const;

    virtual std::string toString() const;

private:
    Type type;
};

} // output

} // event

} // fm

#endif // FM_EVENT_OUTPUT_FACADEEVENT_HPP
