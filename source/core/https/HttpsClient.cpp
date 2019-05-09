#include "core/https/HttpsClient.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <iostream>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
namespace ssl = net::ssl;       // from <boost/asio/ssl.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

using boost::log::trivial::severity_level;

using namespace fm;
using namespace fm::core::https;

HttpsClient::HttpsClient(boost::property_tree::ptree& _configuration, Log _log) :
    log(_log),
    host(_configuration.get<std::string>("core.host")),
    port(_configuration.get<int>("core.port")),
    apiKey(_configuration.get<std::string>("key.apiKey"))
{
}

std::string HttpsClient::execute(const std::string& path, Method method, const std::string& body)
{
    log(severity_level::info, "Executing HTTPS request, path:" + path + " body: " + (body.empty() ? "-" : body));

    constexpr int version = 11;

    // The io_context is required for all I/O
    net::io_context ioc;

    // The SSL context is required, and holds certificates
    ssl::context ctx(ssl::context::sslv23_client);

    // TODO enable certificate verification after tests!
    ctx.set_verify_mode(ssl::verify_none);

    // These objects perform our I/O
    tcp::resolver resolver{ioc};
    ssl::stream<tcp::socket> stream{ioc, ctx};

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if(not SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
    {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
        throw beast::system_error{ec};
    }

    // Look up the domain name
    auto const results = resolver.resolve(host, std::to_string(port));

    // Make the connection on the IP address we get from a lookup
    net::connect(stream.next_layer(), results.begin(), results.end());

    // Perform the SSL handshake
    stream.handshake(ssl::stream_base::client);

    // Set up an HTTP GET request message
    http::verb methodBeast;
    switch (method)
    {
    case PUT: methodBeast = http::verb::put; break;
    case POST: methodBeast = http::verb::post; break;
    case DELETE: methodBeast = http::verb::delete_; break;
    default: methodBeast = http::verb::get;
    }

    http::request<http::string_body> req{methodBeast, path, version};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::authorization, apiKey);
    req.set(http::field::accept, "application/json");

    if (not body.empty())
    {
        req.set(http::field::content_type, "application/json");
        req.set(http::field::content_length, body.size());
        req.body() = body;
    }

    // Send the HTTP request to the remote host
    http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    http::response<http::dynamic_body> res;

    // Receive the HTTP response
    http::read(stream, buffer, res);

    // Gracefully close the stream
    beast::error_code ec;
    stream.shutdown(ec);

    if(ec == net::error::eof)
    {
        // Rationale:
        // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
        ec.assign(0, ec.category());
    }

    if(ec && ec.value() != 1)
    {
        log(severity_level::error, "Executing HTTPS request, path:" + path + " body: " + ec.message());
        throw beast::system_error{ec};
    }

    const http::status code = res.base().result();
    if (http::status::ok <= code && http::status::multiple_choices > code)
    {
        return boost::beast::buffers_to_string(res.body().data());
    }
    else
    {
        std::ostringstream ss;
        ss << "Unexpected server response: "
           << static_cast<int>(code) << " - "  << code
           << ", cause: " << boost::beast::buffers_to_string(res.body().data());
        throw std::runtime_error(ss.str());
    }
}

