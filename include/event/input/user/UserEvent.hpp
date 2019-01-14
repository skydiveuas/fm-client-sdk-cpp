#ifndef FM_EVENT_INPUT_USER_USEREVENT_HPP
#define FM_EVENT_INPUT_USER_USEREVENT_HPP

#include "event/input/IInputEvent.hpp"

#include <string>

namespace fm
{

namespace event
{

namespace input
{

namespace user
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class UserEvent : public IInputEvent
{
public:
    enum Type
    {
        ATTACH,
        RELEASE,
        OPERATE,
        REQUEST_CONTROL,
        RELEASE_ACCEPTED,
        RELEASE_REJECTED,
        CONTROL_READY,
        OPEN_CHANNELS,
        CLOSE_CHANNELS,
    };

    UserEvent(Type);

    virtual ~UserEvent();

    Type getType() const;

    virtual std::string toString() const;

private:
    Type type;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_USEREVENT_HPP
