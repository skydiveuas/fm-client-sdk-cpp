#ifndef FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP
#define FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP

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
class TlsTcpSocket : public TcpSocket
{
public:
    TlsTcpSocket(boost::asio::io_service&);
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_TLSTCPSOCKET_HPP
