#ifndef FM_TRAFFIC_ICHANNEL_HPP
#define FM_TRAFFIC_ICHANNEL_HPP

#include "traffic/socket/ISocket.hpp"

#include <memory>

namespace fm
{

namespace traffic
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-21-21
 * Description:
 */
class IChannel
{
public:
    class Listener
    {
    public:
        virtual ~Listener();

        virtual void onReceived(IChannel*, const socket::ISocket::DataPacket) = 0;

        virtual void onClosed(IChannel*) = 0;
    };

    ~IChannel();

    virtual void setListener(std::shared_ptr<Listener>) = 0;

    virtual void send(const socket::ISocket::DataPacket) = 0;

    virtual long getId() const = 0;

    virtual bool isOwned() const = 0;
};

} // traffic

} // fm

#endif // FM_TRAFFIC_CHANNEL_HPP
