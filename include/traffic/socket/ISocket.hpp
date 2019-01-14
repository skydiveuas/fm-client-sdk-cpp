#ifndef FM_TRAFFIC_SOCKET_ISOCKET_HPP
#define FM_TRAFFIC_SOCKET_ISOCKET_HPP

#include <atomic>
#include <string>

namespace fm
{

namespace traffic
{

namespace socket
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class ISocket
{
public:
    typedef std::pair<const uint8_t* const, const size_t> DataPacket;

    class Listener
    {
    public:
        virtual ~Listener();

        virtual void onReceived(const DataPacket) = 0;

        virtual void onClosed() = 0;
    };

    virtual ~ISocket();

    void setListener(Listener*);

    virtual void connect(const std::string&, const int) = 0;

    virtual size_t readBlocking(uint8_t* buffer, size_t size) = 0;

    virtual void startReading() = 0;

    virtual void send(const DataPacket) = 0;

    virtual void disconnect() = 0;

protected:
    std::atomic<Listener*> listener;
};

} // socket

} // traffic

} // fm

#endif // FM_TRAFFIC_SOCKET_ISOCKET_HPP
