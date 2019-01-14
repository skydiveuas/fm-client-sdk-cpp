#ifndef FM_EVENT_INPUT_CONNECTION_CONNECTIONEVENT_HPP
#define FM_EVENT_INPUT_CONNECTION_CONNECTIONEVENT_HPP

#include "event/input/IInputEvent.hpp"

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
class ConnectionEvent : public IInputEvent
{
public:
    enum Type
    {
        CLOSED,
        ERROR,
        RECEIVED,
        LOST,
    };

    ConnectionEvent(Type);

    virtual ~ConnectionEvent();

    Type getType() const;

    virtual std::string toString() const;

private:
    Type type;
};

} // connection

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_CONNECTION_CONNECTIONEVENT_HPP
