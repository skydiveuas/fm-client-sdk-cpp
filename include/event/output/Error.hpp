#ifndef FM_EVENT_OUTPUT_CHANNELSCLOSED_HPP
#define FM_EVENT_OUTPUT_CHANNELSCLOSED_HPP

#include "event/output/FacadeEvent.hpp"

namespace fm
{

namespace event
{

namespace output
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-09
 * Description:
 */
class Error : public FacadeEvent
{
public:
    Error(const std::string&);

    const std::string& getMessage() const;

    std::string toString() const;

private:
    std::string message;
};

} // output

} // event

} // fm

#endif // FM_EVENT_OUTPUT_CHANNELSCLOSED_HPP
