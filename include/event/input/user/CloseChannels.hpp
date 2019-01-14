#ifndef FM_EVENT_INPUT_USER_CLOSECHANNELS_HPP
#define FM_EVENT_INPUT_USER_CLOSECHANNELS_HPP

#include "event/input/user/UserEvent.hpp"

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
class CloseChannels : public UserEvent
{
public:
    CloseChannels(const std::vector<long>&);

    const std::vector<long>& getChannels() const;

    std::string toString() const override;

private:
    std::vector<long> channels;
};

} // user

} // input

} // event

} // fm

#endif // FM_EVENT_INPUT_USER_CLOSECHANNELS_HPP
