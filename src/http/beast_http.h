#ifndef BEAST_HTTP_HPP
#define BEAST_HTTP_HPP

#include "i_http.h"

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
    class BeastHttp final : public std::unique_ptr<IHttp> {
    public:
        ~BeastHttp() = default;
    }; // namespace yijinc
}
#endif
