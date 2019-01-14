#ifndef FM_EVENT_INPUT_USER_REQUESTCONTROL_HPP
#define FM_EVENT_INPUT_USER_REQUESTCONTROL_HPP

#include "event/input/user/UserEvent.hpp"

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
 * Date: 2018-12-27
 * Description:
 */
class RequestControl : public UserEvent
{
public:
    RequestControl(long);

    long getChannelId() const;

    std::string toString() const override;

private:
    long channelId;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_REQUESTCONTROL_HPP
