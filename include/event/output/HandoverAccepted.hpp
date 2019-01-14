#ifndef FM_EVENT_OUTPUT_HANDOVERACCEPTED_HPP
#define FM_EVENT_OUTPUT_HANDOVERACCEPTED_HPP

#include "event/output/FacadeEvent.hpp"

#include <memory>
#include <vector>

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
class HandoverAccepted : public FacadeEvent
{
public:
    HandoverAccepted(const std::string&);

    const std::vector<uint8_t>& getData() const;

    std::string toString() const;

private:
    std::vector<uint8_t> data;
};

} // output

} // event

} // fm

#endif // FM_EVENT_OUTPUT_HANDOVERACCEPTED_HPP
