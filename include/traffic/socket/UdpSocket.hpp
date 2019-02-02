#ifndef FM_TRAFFIC_SOCKET_UDPSOCKET_HPP
#define FM_TRAFFIC_SOCKET_UDPSOCKET_HPP

#include "traffic/socket/ISocket.hpp"

#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>

#include <deque>
#include <vector>

namespace fm
{

namespace traffic
{

namespace socket
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-01-05
 * Description:
 */
class UdpSocket : public ISocket
{
public:
    UdpSocket(boost::asio::io_service&);

    void connect(const std::string&, const int) override;

    void sendBlocking(const DataPacket) override;

    size_t readBlocking(uint8_t*, size_t) override;

    void startReading() override;

    void send(const DataPacket) override;

    void disconnect() override;

private:
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remoteEndpoint;

    void doRead();
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_UDPSOCKET_HPP
