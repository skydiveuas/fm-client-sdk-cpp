#ifndef FM_ICLIENT_HPP
#define FM_ICLIENT_HPP

#include "event/input/IInputEvent.hpp"

#include "event/output/FacadeEvent.hpp"

#include "core/CoreClient.hpp"

#include "traffic/socket/ISocket.hpp"

#include "system/IStateMachine.hpp"
#include "system/Timer.hpp"

#include "common/channel_management.pb.h"
#include "common/location.pb.h"

#include <boost/asio.hpp>

#include <memory>
#include <mutex>
#include <functional>

namespace fm
{

namespace state
{
    class IState;
}

namespace backend
{
    class ClientBackend;
}

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class IClient : public system::IStateMachine<event::input::IInputEvent>
{
public:
    class Listener
    {
    public:
        virtual ~Listener();

        virtual void onEvent(const std::shared_ptr<const event::output::FacadeEvent>) = 0;

        virtual std::unique_ptr<com::fleetmgr::interfaces::Location> getLocation() = 0;

        virtual void trace(const std::string&) = 0;
    };

    virtual ~IClient();

    void trace(const std::string& message);

    virtual std::string toString() const = 0;

protected:
    std::unique_ptr<backend::ClientBackend> backend;

    IClient(const std::string&, const int, const std::string&, Listener&, boost::asio::io_service&);

private:
    IClient() = delete;
    IClient(IClient&) = delete;

    Listener& listener;
};

} // fm

#endif // FM_IDEVICE_HPP
