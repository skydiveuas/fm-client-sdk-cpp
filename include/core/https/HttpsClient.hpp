#ifndef FM_CORE_HTTPS_HTTPSCLIENT_HPP
#define FM_CORE_HTTPS_HTTPSCLIENT_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/log/trivial.hpp>

#include <string>
#include <functional>

namespace fm
{

namespace core
{

namespace https {

/**
 * Created by: Bartosz Nawrot
 * Date: 2018-11-27
 * Description:
 */
class HttpsClient
{
public:
    typedef std::function<void(const boost::log::trivial::severity_level&, const std::string&)> Log;

    enum Method
    {
        POST,
        PUT,
        GET,
        DELETE,
    };

    HttpsClient(boost::property_tree::ptree&, Log);

    std::string execute(const std::string&, const Method, const std::string&);

protected:
    Log log;

    const std::string host;
    const int port;

    const std::string apiKey;
};

} // https

} // core

} // fm

#endif // FM_CORE_HTTPS_IHTTPSCLIENT_HPP
