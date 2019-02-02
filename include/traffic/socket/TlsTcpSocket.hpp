#ifndef FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP
#define FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP

#include <boost/asio/ssl/stream.hpp>

#include "traffic/socket/TcpSocket.hpp"

namespace fm
{

namespace traffic
{

namespace socket
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-01-29
 * Description:
 */
class TlsTcpSocket : public ISocket
{
public:
    TlsTcpSocket(boost::asio::io_service&);

    void connect(const std::string&, const int) override;

    void sendBlocking(const DataPacket) override;

    size_t readBlocking(uint8_t*, size_t) override;

    void startReading() override;

    void send(const DataPacket) override;

    void disconnect() override;

private:
    boost::asio::ssl::context sslContext;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket;

    void doRead();
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP
