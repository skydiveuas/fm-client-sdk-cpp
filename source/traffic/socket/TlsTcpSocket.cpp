#include "traffic/socket/TlsTcpSocket.hpp"

#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <iostream>

using namespace fm;
using namespace fm::traffic::socket;

using namespace boost::asio;

TlsTcpSocket::TlsTcpSocket(boost::asio::io_service& _ioService) :
    ioService(_ioService),
    sslContext(boost::asio::ssl::context::tls),
    socket(ioService, sslContext)
{
}

void TlsTcpSocket::connect(const std::string& host, const int port)
{
    //std::cout << "TlsTcpSocket::connect(" << host << ":" << port << ")" << std::endl;
    // TODO enable certificate verification after tests!
    socket.set_verify_mode(ssl::verify_none);

    ip::tcp::resolver resolver(ioService);
    auto endpoint = resolver.resolve(host, std::to_string(port));
    socket.lowest_layer().connect(*endpoint);

    socket.handshake(ssl::stream<boost::asio::ip::tcp::socket>::client);

    std::cout << "TlsTcpSocket::connect done" << std::endl;
}

void TlsTcpSocket::sendBlocking(const DataPacket dataPacket)
{
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    size_t sent = socket.write_some(buffer(sendBuffer.back().data(), sendBuffer.back().size()));
    if (dataPacket.second != sent)
    {
        throw std::runtime_error("Could not send enough data");
    }
    sendBuffer.pop_front();
}


size_t TlsTcpSocket::readBlocking(uint8_t* _buffer, size_t _size)
{
    return socket.read_some(buffer(_buffer, _size));
}

void TlsTcpSocket::startReading()
{
    doRead();
}

void TlsTcpSocket::send(const DataPacket dataPacket)
{
    //std::cout << "TlsTcpSocket::send" << std::endl;
    sendBuffer.emplace_back(dataPacket.first, dataPacket.first + dataPacket.second);
    socket.async_write_some(buffer(sendBuffer.back().data(), sendBuffer.back().size()),
                [this](boost::system::error_code ec, std::size_t /*length*/)
    {
        sendBuffer.pop_front();
        if (ec)
        {
            std::cout << "TlsTcpSocket::send error: " + ec.message() << std::endl;
        }
    });
}

void TlsTcpSocket::disconnect()
{
    //std::cout << "TlsTcpSocket::disconnect" << std::endl;
    listener.load()->onClosed();
}

void TlsTcpSocket::doRead()
{
    //std::cout << "TlsTcpSocket::doRead" << std::endl;
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
            std::cout << "TlsTcpSocket::doRead error: " + ec.message() << std::endl;
        }
    });
}
