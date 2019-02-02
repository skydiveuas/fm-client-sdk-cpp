#include "backend/ChannelsHandler.hpp"

#include "backend/ClientBackend.hpp"

#include "traffic/socket/TcpSocket.hpp"
#include "traffic/socket/TlsTcpSocket.hpp"
#include "traffic/socket/UdpSocket.hpp"

using namespace fm;
using namespace fm::backend;
using namespace fm::traffic::socket;

using namespace com::fleetmgr::interfaces;

using namespace boost::asio;

ChannelsHandler::ChannelsHandler(ClientBackend& _backend) :
    backend(_backend),
    sslContext(boost::asio::ssl::context::tls)
{
    // TODO enable certificate verification after tests!
    sslContext.set_verify_mode(ssl::verify_none);
}

std::vector<traffic::IChannel*> ChannelsHandler::getChannels()
{
    std::vector<traffic::IChannel*> result;
    result.reserve(channels.size());
    for (auto& pair : channels)
    {
        result.push_back(&(pair.second));
    }
    return result;
}

std::vector<traffic::IChannel*> ChannelsHandler::getChannels(const std::vector<long>& ids)
{
    std::vector<traffic::IChannel*> result;
    result.reserve(ids.size());
    for (long id : ids)
    {
        result.push_back(&channels.find(id)->second);
    }
    return result;
}

std::vector<long> ChannelsHandler::getChannelsIds()
{
    std::vector<long> result;
    result.reserve(channels.size());
    for (auto& pair : channels)
    {
        result.push_back(pair.first);
    }
    return result;
}

std::vector<traffic::IChannel*> ChannelsHandler::validateChannels(const std::vector<ChannelResponse>& toValidate)
{
    std::vector<traffic::IChannel*> result;
    for (const ChannelResponse& c : toValidate)
    {
        trace("Opening channel id: " + std::to_string(c.id()));
        std::shared_ptr<ISocket> socket = buildSocket(c);
        auto pair = channels.emplace(std::piecewise_construct,
                                     std::forward_as_tuple(c.id()),
                                     std::forward_as_tuple(c.id(), socket));
        traffic::ChannelImpl* channel = &pair.first->second;
        if (channel->open(c.host(), c.port(), c.key()))
        {
            trace("Channel id: " + std::to_string(c.id()) + " validated");
            result.push_back(channel);
        }
        else
        {
            trace("Error!, Could not validate channel id: " + std::to_string(c.id()));
            channels.erase(c.id());
        }
    }
    return result;
}

void ChannelsHandler::closeChannels(const std::vector<long>& toClose)
{
    for (long id : toClose)
    {
        auto c = channels.find(id);
        if (c != channels.end())
        {
            trace("Closing channel, id: " + std::to_string(id));
            c->second.close();
            channels.erase(c);
        }
        else
        {
            trace("Warning, trying to close not existing channel, id: " + std::to_string(id));
        }
    }
}

void ChannelsHandler::closeAllChannels()
{
    for (auto& pair : channels)
    {
        trace("Closing channel, id: " + std::to_string(pair.first));
        pair.second.close();
    }
    channels.clear();
}

void ChannelsHandler::setOwned(const std::vector<long>& owned)
{
    for (long id : owned)
    {
        trace("Setting channel, id: " + std::to_string(id) + " as owned");
        channels.find(id)->second.setOwned(true);
    }
}

void ChannelsHandler::setOwned(long id, bool owned)
{
    channels.find(id)->second.setOwned(owned);
}

void ChannelsHandler::trace(const std::string& message)
{
    backend.trace(message);
}

std::shared_ptr<ISocket> ChannelsHandler::buildSocket(const ChannelResponse& parameters)
{
    switch (parameters.protocol())
    {
    case Protocol::UDP:
        switch (parameters.security())
        {
        case Security::PLAIN_TEXT:
            return std::make_unique<UdpSocket>(backend.getIoService());

        case Security::TLS:
            throw std::runtime_error("UDP encryption not supported");
        }
        break;

    case Protocol::TCP:
        switch (parameters.security())
        {
        case Security::PLAIN_TEXT:
            return std::make_unique<TcpSocket>(backend.getIoService());

        case Security::TLS:
            return std::make_unique<TlsTcpSocket>(backend.getIoService(), sslContext);
        }
        break;

    default:
        throw std::runtime_error("Unexpected Protocol or Security type");
    }
}
