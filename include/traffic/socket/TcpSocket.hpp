#ifndef FM_TRAFFIC_SOCKET_TCPSOCKET_HPP
#define FM_TRAFFIC_SOCKET_TCPSOCKET_HPP

#include "traffic/socket/ISocket.hpp"

#include <boost/asio.hpp>

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
class TcpSocket : public ISocket
{
public:
    TcpSocket(boost::asio::io_service&);

    void connect(const std::string&, const int) override;

    void sendBlocking(const DataPacket) override;

    size_t readBlocking(uint8_t*, size_t) override;

    void startReading() override;

    void send(const DataPacket) override;

    void disconnect() override;

private:
    boost::asio::ip::tcp::socket socket;

    void doRead();
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_TCPSOCKET_HPP
