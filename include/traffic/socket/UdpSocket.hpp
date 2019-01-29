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
    // TODO Bartek change buffers to more memory efficnient structures
    // TODO Bartek for example boost::circular_buffer
    std::deque<std::vector<uint8_t>> sendBuffer;
    std::deque<std::vector<uint8_t>> readBuffer;

    boost::asio::io_service& ioService;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint remoteEndpoint;

    void doRead();
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_UDPSOCKET_HPP
