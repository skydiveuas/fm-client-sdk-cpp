#ifndef ISIMULATOR_HPP
#define ISIMULATOR_HPP

#include "traffic/IChannel.hpp"

#include "Device.hpp"

#include "event/output/ChannelsOpened.hpp"

#include <deque>
#include <mutex>

class ISimulator : public fm::IClient::Listener
{
public:
    class ChannelListener : public fm::traffic::IChannel::Listener
    {
    public:
        ChannelListener(std::unordered_map<long, fm::traffic::IChannel*>&, std::mutex&);

        void onReceived(fm::traffic::IChannel*, const fm::traffic::socket::ISocket::DataPacket) override;

        void onClosed(fm::traffic::IChannel*) override;

    private:
        std::unordered_map<long, fm::traffic::IChannel*>& channels;
        std::mutex& channelsLock;
    };

    ISimulator(boost::asio::io_service&);

    virtual ~ISimulator();

    bool isDone();

    void onEvent(const std::shared_ptr<const fm::event::output::FacadeEvent> event) override;

    std::unique_ptr<com::fleetmgr::interfaces::Location> getLocation() override;

    void trace(const std::string&) override;

protected:
    boost::asio::io_service& ioService;

    std::atomic<bool> done;

    virtual void handleEvent(const std::shared_ptr<const fm::event::output::FacadeEvent> event) = 0;

private:
    std::unordered_map<long, fm::traffic::IChannel*> channels;
    std::mutex channelsLock;

    std::thread trafficThread;

    void addChannels(const fm::event::output::ChannelsOpened&);

    void trafficSimulator();

    static std::string timestamp();
};

#endif // ISIMULATOR_HPP
