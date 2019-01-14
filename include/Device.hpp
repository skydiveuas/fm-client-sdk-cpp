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
    Device(const std::string&, const int, const std::string&, Listener&, boost::asio::io_service&);

    std::string toString() const override;
};

} // fm

#endif // FM_DEVICE_HPP
