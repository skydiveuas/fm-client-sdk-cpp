#include "system/Timer.hpp"

#include <boost/bind.hpp>

using namespace fm::system;

Timer::Timer(boost::asio::io_service& io, Task _task, size_t timeout, size_t _id, Cancel _cancelationCallback) :
    id(_id),
    task(_task),
    cancelationCallback(_cancelationCallback),
    timer(io, boost::asio::chrono::milliseconds(timeout)),
    interval(0),
    recurring(false),
    canceled(false)
{
    timer.async_wait(boost::bind(&Timer::timerTask, this, boost::asio::placeholders::error));
}

Timer::Timer(boost::asio::io_service& io, Task _task, size_t delay, size_t _interval, size_t _id, Cancel _cancelationCallback) :
    id(_id),
    task(_task),
    cancelationCallback(_cancelationCallback),
    timer(io, boost::asio::chrono::milliseconds(delay)),
    interval(_interval),
    recurring(true),
    canceled(false)
{
    timer.async_wait(boost::bind(&Timer::timerTask, this, boost::asio::placeholders::error));
}

void Timer::cancel()
{
    if (not canceled)
    {
        canceled = true;
        timer.cancel();
        cancelationCallback(id);
    }
}

size_t Timer::getId() const
{
    return id;
}

void Timer::timerTask(const boost::system::error_code& ec)
{
    if (boost::asio::error::operation_aborted == ec)
    {
        return;
    }

    task();

    if (recurring)
    {
        timer.expires_at(timer.expiry() + boost::asio::chrono::milliseconds(interval));
    }
}
