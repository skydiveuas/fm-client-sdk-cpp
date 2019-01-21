#ifndef FM_SYSTEM_TIMER_HPP
#define FM_SYSTEM_TIMER_HPP

#include <boost/asio.hpp>

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
    typedef std::function<void(size_t)> Cancel;

    Timer(boost::asio::io_service&, Task, size_t, size_t, Cancel);

    Timer(boost::asio::io_service&, Task, size_t, size_t, size_t, Cancel);

    void cancel();

    size_t getId() const;

private:
    const size_t id;
    const Task task;
    const Cancel cancelationCallback;

    boost::asio::steady_timer timer;

    const size_t interval;
    const bool recurring;

    bool canceled;

    void timerTask(const boost::system::error_code&);
};

} // system

} // fm

#endif // FM_SYSTEM_TIMER_HPP
