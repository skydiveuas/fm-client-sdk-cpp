#ifndef FM_EVENT_INPUT_USER_RELEASEREJECTED_HPP
#define FM_EVENT_INPUT_USER_RELEASEREJECTED_HPP

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
 * Date: 2018-12-09
 * Description:
 */
class ReleaseRejected : public UserEvent
{
public:
    ReleaseRejected(const std::string&);

    const std::string& getMessage() const;

    std::string toString() const override;

private:
    std::string message;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_RELEASEREJECTED_HPP
