#ifndef FM_EVENT_INPUT_IINPUTEVENT_HPP
#define FM_EVENT_INPUT_IINPUTEVENT_HPP

#include <string>

namespace fm
{

namespace event
{

namespace input
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-06
 * Description:
 */
class IInputEvent
{
public:
    enum InputEventType
    {
        USER_EVENT,
        CONNECTION_EVENT,
    };

    IInputEvent(InputEventType);

    virtual ~IInputEvent();

    InputEventType getInputEventType() const;

    virtual std::string toString() const = 0;

private:
    InputEventType inputEventType;
};

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_IINPUTEVENT_HPP
