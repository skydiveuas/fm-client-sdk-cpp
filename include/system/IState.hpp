#ifndef FM_SYSTEM_ISTATE_HPP
#define FM_SYSTEM_ISTATE_HPP

#include <memory>

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
class IState
{
public:
    virtual ~IState()
    {
    }

    virtual std::unique_ptr<IState<_Event>> start() = 0;

    virtual std::unique_ptr<IState<_Event>> handleEvent(const std::shared_ptr<const _Event>) = 0;

    virtual std::string toString() const = 0;
};

} // system

} // fm

#endif // FM_SYSTEM_ISTATE_HPP
