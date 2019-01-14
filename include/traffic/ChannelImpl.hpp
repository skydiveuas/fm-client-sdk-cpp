#ifndef FM_TRAFFIC_CHANNELIMPL_HPP
#define FM_TRAFFIC_CHANNELIMPL_HPP

#include "traffic/IChannel.hpp"

#include <memory>
#include <string>

namespace fm
{

namespace traffic
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class ChannelImpl : public IChannel, socket::ISocket::Listener
{
public:
    ChannelImpl(long, std::shared_ptr<socket::ISocket>);

    void setListener(std::shared_ptr<IChannel::Listener>) override;

    bool open(const std::string&, const int, const std::string&);

    void close();

    void setOwned(const bool);

    void send(const socket::ISocket::DataPacket) override;

    long getId() const override;

    bool isOwned() const override;

private:
    const long id;
    std::shared_ptr<socket::ISocket> socket;

    std::shared_ptr<IChannel::Listener> listener;

    bool owned;

    void onReceived(const socket::ISocket::DataPacket) override;

    void onClosed() override;
};

} // traffic

} // fm

#endif // FM_TRAFFIC_CHANNELIMPL_HPP
