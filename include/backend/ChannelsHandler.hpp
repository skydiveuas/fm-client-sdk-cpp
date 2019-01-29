#ifndef FM_BACKEND_CHANNELSHANDLER_HPP
#define FM_BACKEND_CHANNELSHANDLER_HPP

#include "traffic/ChannelImpl.hpp"

#include "common/channel_management.pb.h"

#include <memory>
#include <vector>
#include <unordered_map>

namespace fm
{

namespace backend
{

class ClientBackend;

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-12-16
 * Description:
 */
class ChannelsHandler
{
public:
    ChannelsHandler(ClientBackend&);

    std::vector<traffic::IChannel*> getChannels();

    std::vector<traffic::IChannel*> getChannels(const std::vector<long>&);

    std::vector<long> getChannelsIds();

    std::vector<traffic::IChannel*> validateChannels(const std::vector<com::fleetmgr::interfaces::ChannelResponse>&);

    void closeChannels(const std::vector<long>&);

    void closeAllChannels();

    void setOwned(const std::vector<long>&);

    void setOwned(long, bool);

private:
    ClientBackend& backend;

    std::unordered_map<long, traffic::ChannelImpl> channels;

    void trace(const std::string& message);

    std::shared_ptr<traffic::socket::ISocket> buildSocket(const com::fleetmgr::interfaces::ChannelResponse&) const;
};

} // backend

} // fm

#endif // FM_BACKEND_CHANNELSHANDLER_HPP
