#ifndef FM_SYSTEM_TIMER_HPP
#define FM_SYSTEM_TIMER_HPP

#include <functional>

namespace fm
{

namespace system
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class Timer
{
public:
    typedef std::function<void(void)> Task;
    typedef size_t Milisec;

    virtual ~Timer();
};

} // system

} // fm

#endif // FM_SYSTEM_TIMER_HPP
