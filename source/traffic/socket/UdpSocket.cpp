#include "traffic/socket/UdpSocket.hpp"

#include <iostream>

using namespace fm;
using namespace fm::traffic::socket;

using namespace boost::asio;

UdpSocket::UdpSocket(boost::asio::io_service& _ioService) :
    ioService(_ioService),
    socket(ioService, ip::udp::endpoint(ip::udp::v4(), 0))
{
}

void UdpSocket::connect(const std::string& host, const int port)
{
    //std::cout << "UdpSocket::connect(" << host << ":" << port << ")" << std::endl;
    ip::udp::resolver resolver(ioService);
    remoteEndpoint = *resolver.resolve(host, std::to_string(port));
}

void UdpSocket::sendBlocking(const DataPacket dataPacket)
{
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    size_t sent = socket.send_to(buffer(sendBuffer.back().data(), sendBuffer.back().size()), remoteEndpoint);
    if (dataPacket.second != sent)
    {
        throw std::runtime_error("Could not send enough data");
    }
    sendBuffer.pop_front();
}

size_t UdpSocket::readBlocking(uint8_t* _buffer, size_t _size)
{
    return socket.receive_from(buffer(_buffer, _size), remoteEndpoint);
}

void UdpSocket::startReading()
{
    doRead();
}

void UdpSocket::send(const DataPacket dataPacket)
{
    //std::cout << "UdpSocket::send" << std::endl;
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    socket.async_send_to(buffer(sendBuffer.back().data(), sendBuffer.back().size()), remoteEndpoint,
                         [this](boost::system::error_code ec, std::size_t /*length*/)
    {
        sendBuffer.pop_front();
        if (ec)
        {
            std::cout << "UdpSocket::send error: " + ec.message() << std::endl;
        }
    });
}

void UdpSocket::disconnect()
{
    //std::cout << "UdpSocket::disconnect" << std::endl;
    listener.load()->onClosed();
}

void UdpSocket::doRead()
{
    //std::cout << "UdpSocket::doRead" << std::endl;
    readBuffer.emplace_back(1024);
    socket.async_receive_from(buffer(readBuffer.back(), 1024), remoteEndpoint,
                              [this](boost::system::error_code ec, std::size_t length)
    {
        if (!ec)
        {
            listener.load()->onReceived(DataPacket(readBuffer.front().data(), length));
            readBuffer.pop_front();
            doRead();
        }
        else
        {
            std::cout << "UdpSocket::doRead error: " + ec.message() << std::endl;
        }
    });
}
