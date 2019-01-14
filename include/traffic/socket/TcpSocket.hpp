#ifndef FM_TRAFFIC_SOCKET_TCPSOCKET_HPP
#define FM_TRAFFIC_SOCKET_TCPSOCKET_HPP

#include "traffic/socket/ISocket.hpp"

#include <boost/asio.hpp>

#include <array>

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
class TcpSocket : public traffic::socket::ISocket
{
public:
    TcpSocket(boost::asio::io_service&);

    void connect(const std::string&, const int) override;

    size_t readBlocking(uint8_t*, size_t) override;

    void startReading() override;

    void send(const DataPacket) override;

    void disconnect() override;

private:
    // TODO Bartek change arrays to RoundBuffers
    static constexpr size_t READ_BUFFER_SIZE = 1024;
    std::array<uint8_t, READ_BUFFER_SIZE> readBuffer;

    static constexpr size_t SEND_BUFFER_SIZE = 1024;
    std::array<uint8_t, SEND_BUFFER_SIZE> sendBuffer;

    boost::asio::io_service& ioService;
    boost::asio::ip::tcp::socket socket;

    void doRead();
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_TCPSOCKET_HPP
