#include "traffic/socket/ISocket.hpp"

#include <vector>

using namespace fm::traffic::socket;

ISocket::Listener::~Listener()
{
}

ISocket::ISocket(boost::asio::io_service& _ioService) :
    ioService(_ioService)
{
}

ISocket::~ISocket()
{
}

void ISocket::setListener(Listener* _listener)
{
    listener = _listener;
}
