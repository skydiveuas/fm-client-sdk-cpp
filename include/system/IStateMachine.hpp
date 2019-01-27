#ifndef FM_SYSTEM_ISTATEMACHINE_HPP
#define FM_SYSTEM_ISTATEMACHINE_HPP

#include "system/IState.hpp"
#include "system/Timer.hpp"

#include <boost/asio.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <deque>
#include <atomic>

namespace fm
{

namespace system
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-01-04
 * Description:
 */
template <typename _Event>
class IStateMachine
{
public:
    typedef std::function<void(void)> Task;
    typedef std::function<void(const std::string&)> Log;

    IStateMachine(Log _log, boost::asio::io_service& _ioService) :
        log(_log),
        ioService(_ioService),
        processing(false)
    {
    }

    virtual ~IStateMachine()
    {
    }

    void notifyEvent(const std::shared_ptr<const _Event> event)
    {
        std::unique_lock<std::mutex> guard(lock);
        if (processing.load())
        {
            queue.push_back([this, event] () {
                handleEvent(event);
                proceed();
            });
        }
        else
        {
            processing.store(true);
            execute([this, event] () {
                handleEvent(event);
                proceed();
            });
        }
    }

    std::shared_ptr<Timer> executeAfter(Task task, size_t timeout)
    {
        return std::make_shared<Timer>(task, timeout);
    }

    std::shared_ptr<Timer> executeEvery(Task task, size_t delay, size_t interval)
    {
        return std::make_shared<Timer>(task, delay, interval);
    }

    void defer(const std::shared_ptr<const _Event> event)
    {
        log("Deffering: " + event->toString() + " @ " + state->toString());
        deferred.push_back(event);
    }

    void recall()
    {
        if (not deferred.empty())
        {
            std::shared_ptr<const _Event> event = deferred.front();
            deferred.pop_front();
            log("Recalling: " + event->toString() + " @ " + state->toString() + ", remaining deferred events: " + std::to_string(deferred.size()));
            notifyEvent(event);
        }
    }

    std::string getStateName() const
    {
        return state->toString();
    }

protected:
    void setState(std::unique_ptr<IState<_Event>> _state)
    {
        state.swap(_state);
        state->start();
    }

private:
    static constexpr size_t NOT_TIMER_ID = 0;

    Log log;

    boost::asio::io_service& ioService;

    std::mutex lock;
    std::deque<Task> queue;
    std::atomic<bool> processing;

    std::unique_ptr<IState<_Event>> state;
    std::deque<std::shared_ptr<const _Event>> deferred;

    void execute(Task task)
    {
        ioService.post(task);
    }

    void proceed()
    {
        std::unique_lock<std::mutex> guard(lock);
        if (not queue.empty())
        {
            Task task = queue.front();
            queue.pop_front();
            execute(task);
        }
        else
        {
            processing.store(false);
        }
    }

    void handleEvent(const std::shared_ptr<const _Event> event)
    {
        log("Handling: " + event->toString() + " @ " + state->toString());
        std::unique_ptr<IState<_Event>> newState = state->handleEvent(event);
        while (nullptr != newState.get())
        {
            log("Transition: " + state->toString() + " -> " + newState->toString());
            state.swap(newState);
            newState.reset(state->start().release());
        }
    }
};

} // system

} // fm

#endif // FM_SYSTEM_ISTATEMACHINE_HPP
