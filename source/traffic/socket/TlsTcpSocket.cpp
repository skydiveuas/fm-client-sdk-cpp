#include "traffic/socket/TlsTcpSocket.hpp"

#include <iostream>

using namespace fm;
using namespace fm::traffic::socket;

using namespace boost::asio;

TlsTcpSocket::TlsTcpSocket(boost::asio::io_service& ioService) :
    TcpSocket(ioService)
{
}
