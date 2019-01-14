#ifndef FM_EVENT_OUTPUT_RELEASECONTROL_HPP
#define FM_EVENT_OUTPUT_RELEASECONTROL_HPP

#include "event/output/FacadeEvent.hpp"

namespace fm
{

namespace event
{

namespace output
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-27
 * Description:
 */
class ReleaseControl : public FacadeEvent
{
public:
    ReleaseControl(long);

    long getChannelId() const;

    std::string toString() const override;

private:
    long channelId;
};

} // output

} // event

} // fm

#endif // FM_EVENT_OUTPUT_RELEASECONTROL_HPP
