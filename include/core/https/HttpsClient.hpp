#ifndef FM_CORE_HTTPS_HTTPSCLIENT_HPP
#define FM_CORE_HTTPS_HTTPSCLIENT_HPP

#include <string>

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
    enum Method
    {
        POST,
        PUT,
        GET,
        DELETE,
    };

    HttpsClient(const std::string&, const int, const std::string&);

    std::string execute(const std::string&, const Method, const std::string&);

protected:
    const std::string host;
    const int port;

    const std::string apiKey;
};

} // https

} // core

} // fm

#endif // FM_CORE_HTTPS_IHTTPSCLIENT_HPP
