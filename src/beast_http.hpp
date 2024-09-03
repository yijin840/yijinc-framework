#ifndef BEAST_HTTP_HPP
#define BEAST_HTTP_HPP

#include "i_http.hpp"

#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

#include <boost/url.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace urls = boost::urls;
using tcp = net::ip::tcp;

namespace yijinc {
class BeastHttp final : public IHttp {
public:
    static std::string proxy_host;
    static std::string proxy_port;
    static bool proxy_enable;
    static std::string
        get(const std::string& url,
            const std::map<std::string, std::string>& headers,
            const std::map<std::string, std::string>& params);

    static std::string
        post(const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body);
    static void set_proxy(const std::string& host, const std::string& port);
    static std::string to_query_params(const std::map<std::string, std::string>& params);

    static void init_ssl() {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
    }

    static std::string curl_get(
        const std::string& url,
        const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& params);
    static std::string
        curl_post(const std::string& url, const std::map<std::string, std::string>& headers, const std::string& body);

private:
    static std::string request(
        const std::string& url,
        const std::map<std::string, std::string>& headers,
        const std::string& body,
        const http::verb& method);
    static http::response<http::string_body>
        https_request(net::io_context& ioc, const http::verb& method, http::request<http::string_body>& req);
    static http::response<http::string_body>
        http_request(net::io_context& ioc, const http::verb& method, http::request<http::string_body>& req);
    static void close_stream(boost::beast::ssl_stream<boost::beast::tcp_stream>& ssl_stream);
    static void close_stream(boost::beast::tcp_stream& tcp_stream);
};

} // namespace yijinc

#endif
