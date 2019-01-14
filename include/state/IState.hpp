#ifndef FM_STATE_ISTATE_HPP
#define FM_STATE_ISTATE_HPP

#include "core/CoreClient.hpp"
#include "IClient.hpp"

#include "backend/HeartbeatHandler.hpp"

#include "event/input/user/UserEvent.hpp"
#include "event/input/connection/ConnectionEvent.hpp"

#include "facade/control/facade_service.pb.h"

#include "system/IState.hpp"

#include <memory>

namespace fm
{

namespace state
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class IState : public system::IState<event::input::IInputEvent>
{
public:
    typedef std::unique_ptr<system::IState<event::input::IInputEvent>> State;

    virtual ~IState();

    State handleEvent(const std::shared_ptr<const event::input::IInputEvent>) override;

    virtual State handleUserEvent(const event::input::user::UserEvent&);

    virtual State handleConnectionEvent(const event::input::connection::ConnectionEvent&);

    virtual State createOuterState();

    virtual std::string toString() const = 0;

protected:
    IClient& client;
    IClient::Listener& listener;

    backend::ClientBackend& backend;

    IState(IState&);

    IState(IClient&, IClient::Listener&, backend::ClientBackend&);

    void send(const com::fleetmgr::interfaces::facade::control::ClientMessage&);

    State defaultEventHandle(const std::string& eventName);

    State defaultMessageHandle(const com::fleetmgr::interfaces::facade::control::ControlMessage&);

    void trace(const std::string& message);
};

} // state

} // fm

#endif // FM_STATE_STATE_HPP
