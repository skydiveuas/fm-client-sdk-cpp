#include "ISimulator.hpp"

#include "event/input/user/UserEvent.hpp"

#include <ctime>
#include <chrono>
#include <iomanip>

using namespace fm;
using namespace fm::event;

using fm::event::input::user::UserEvent;
using fm::event::output::FacadeEvent;
using fm::event::output::ChannelsOpened;

using com::fleetmgr::interfaces::Location;

ISimulator::ChannelListener::ChannelListener(std::unordered_map<long, fm::traffic::IChannel*>& _channels, std::mutex& _channelsLock) :
    channels(_channels),
    channelsLock(_channelsLock)
{
}

void ISimulator::ChannelListener::onReceived(fm::traffic::IChannel* channel, const fm::traffic::socket::ISocket::DataPacket dataPacket)
{
    std::string message(reinterpret_cast<const char*>(dataPacket.first), dataPacket.second);
    std::cout << ISimulator::timestamp() << " Received[" << channel->getId() << "]: " << message << std::endl;
}

void ISimulator::ChannelListener::onClosed(fm::traffic::IChannel* channel)
{
    std::lock_guard<std::mutex> lock(channelsLock);
    auto c = channels.find(channel->getId());
    if (c != channels.end())
    {
        std::cout << "Removing channel, id: " << channel->getId() << std::endl;
        channels.erase(c);
    }
    else
    {
        std::cout << "Closed not existing channel, id: " << channel->getId() << std::endl;
    }
}

ISimulator::ISimulator(boost::asio::io_service& _ioService) :
    ioService(_ioService),
    done(false)
{
}

ISimulator::~ISimulator()
{
    if (trafficThread.joinable())
    {
        trafficThread.join();
    }
}

bool ISimulator::isDone()
{
    return done.load();
}

void ISimulator::onEvent(const std::shared_ptr<const FacadeEvent> event)
{
    trace("Handling FacadeEvent: " + event->toString());
    switch (event->getType())
    {
    case FacadeEvent::CHANNELS_OPENED:
        addChannels(reinterpret_cast<const ChannelsOpened&>(*event));
        break;
    }
    handleEvent(event);
}

std::unique_ptr<Location> ISimulator::getLocation()
{
    std::unique_ptr<Location> location = std::make_unique<Location>();
    location->set_latitude(50.0);
    location->set_longitude(20.0);
    location->set_altitude(400.0);
    return location;
}

void ISimulator::trace(const std::string& message)
{
    std::cout << timestamp() << " Client trace: [" << message << "]" << std::endl;
}

void ISimulator::addChannels(const ChannelsOpened& event)
{
    bool wasEmpty = channels.empty();
    for (traffic::IChannel* c : event.getChannels())
    {
        std::shared_ptr<ChannelListener> listener = std::make_shared<ChannelListener>(channels, channelsLock);
        c->setListener(listener);
        channels.insert({c->getId(), c});
    }
    if (wasEmpty)
    {
        trafficThread = std::thread(&ISimulator::trafficSimulator, this);
    }
}

void ISimulator::trafficSimulator()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lock(channelsLock);
        if (channels.empty())
        {
            break;
        }
        for (auto& pair : channels)
        {
            std::string message("Channel id: " + std::to_string(pair.first) + " test message " + std::to_string(clock()));
            std::cout << ISimulator::timestamp() << "  Sending[" << std::to_string(pair.first) << "]: " << message << std::endl;
            pair.second->send(traffic::socket::ISocket::DataPacket(reinterpret_cast<const uint8_t*>(message.data()), message.size()));
        }
    }
    trace("Closing traffic thread");
}

std::string ISimulator::timestamp()
{
    // Bartek WTF is this??
    static constexpr auto milisFormat = "%03d";
    static constexpr auto timeFormat = "%H.%M.%S:";
    static char ms[4] = "";
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const auto fraction = now - std::chrono::time_point_cast<std::chrono::seconds>(now);
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
    const time_t cnow = std::chrono::system_clock::to_time_t(now);
    sprintf(ms, milisFormat, static_cast<int>(milliseconds.count()));
    std::stringstream ss;
    ss << std::put_time(std::localtime(&cnow), timeFormat) << std::string(ms);
    return ss.str();
}
