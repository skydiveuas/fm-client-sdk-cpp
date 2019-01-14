#ifndef FM_EVENT_INPUT_USER_RELEASEACCEPTED_HPP
#define FM_EVENT_INPUT_USER_RELEASEACCEPTED_HPP

#include "event/input/user/UserEvent.hpp"

#include <memory>
#include <vector>

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
class ReleaseAccepted : public UserEvent
{
public:
    ReleaseAccepted(const std::vector<uint8_t>&);

    const std::vector<uint8_t>& getData() const;

    std::string toString() const override;

private:
    std::vector<uint8_t> data;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_RELEASEACCEPTED_HPP
