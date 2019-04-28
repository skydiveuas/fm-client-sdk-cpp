#ifndef FM_DEVICE_HPP
#define FM_DEVICE_HPP

#include "IClient.hpp"

namespace fm
{

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-25
 * Description:
 */
class Device : public IClient
{
public:
    Device(boost::asio::io_service&, boost::property_tree::ptree&, Listener&);

    Device(boost::asio::io_service&, const std::string&, Listener&);

    std::string toString() const override;
};

} // fm

#endif // FM_DEVICE_HPP
