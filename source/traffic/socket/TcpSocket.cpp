#include "traffic/socket/TcpSocket.hpp"

#include <iostream>

using namespace fm;
using namespace fm::traffic::socket;

using namespace boost::asio;

TcpSocket::TcpSocket(boost::asio::io_service& _ioService) :
    ISocket(_ioService),
    socket(ioService)
{
}

void TcpSocket::connect(const std::string& host, const int port)
{
    //std::cout << "TcpSocket::connect(" << host << ":" << port << ")" << std::endl;
    ip::tcp::resolver resolver(ioService);
    auto endpoint = resolver.resolve(host, std::to_string(port));
    boost::asio::connect(socket, endpoint);
}

void TcpSocket::sendBlocking(const DataPacket dataPacket)
{
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    size_t sent = socket.write_some(buffer(sendBuffer.back().data(), sendBuffer.back().size()));
    if (dataPacket.second != sent)
    {
        throw std::runtime_error("Could not send enough data");
    }
    sendBuffer.pop_front();
}


size_t TcpSocket::readBlocking(uint8_t* _buffer, size_t _size)
{
    return socket.read_some(buffer(_buffer, _size));
}

void TcpSocket::startReading()
{
    doRead();
}

void TcpSocket::send(const DataPacket dataPacket)
{
    //std::cout << "TcpSocket::send" << std::endl;
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    socket.async_write_some(buffer(sendBuffer.back().data(), sendBuffer.back().size()),
                [this](boost::system::error_code ec, std::size_t /*length*/)
    {
        sendBuffer.pop_front();
        if (ec)
        {
            std::cout << "TcpSocket::send error: " + ec.message() << std::endl;
        }
    });
}

void TcpSocket::disconnect()
{
    //std::cout << "TcpSocket::disconnect" << std::endl;
    listener.load()->onClosed();
}

void TcpSocket::doRead()
{
    //std::cout << "TcpSocket::doRead" << std::endl;
    readBuffer.emplace_back(1024);
    socket.async_read_some(buffer(readBuffer.back(), 1024),
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
            std::cout << "TcpSocket::doRead error: " + ec.message() << std::endl;
        }
    });
}
