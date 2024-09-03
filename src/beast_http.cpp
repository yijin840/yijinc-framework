#include "beast_http.hpp"
#include "cipher.hpp"
#include "i_http.hpp"
#include "logger.hpp"
#include <curl/curl.h>
#include <vector>
#define DEBUG_ENABLE false
#define CONNECT_TIMEOUT 20

using logger = yijinc::Logger;
using cipher = yijinc::Cipher;

// 定义静态成员变量
std::string yijinc::BeastHttp::get(
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& params) {
    if constexpr (DEBUG_ENABLE) {
        logger::info("http get url: {}", url);
        for (const auto& [fst, snd] : headers) {
            logger::info("http get headers: {} => {}", fst, snd);
        }
        for (const auto& [fst, snd] : params) {
            logger::info("http get params: {} => {}", fst, snd);
        }
    }
    // url需要自己处理
    const std::string query_params = to_query_params(params);
    std::string format_url = url;
    if (!query_params.empty()) {
        format_url += "?" + query_params;
    }
    return request(format_url, headers, "", http::verb::get);
}

std::string yijinc::BeastHttp::post(
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::string& body) {
    if constexpr (DEBUG_ENABLE) {
        logger::info("http post url: {}", url);
        logger::info("http post body: {}", body);
    }
    return request(url, headers, body, http::verb::post);
}

std::string yijinc::BeastHttp::to_query_params(const std::map<std::string, std::string>& params) {
    if (params.empty()) {
        return "";
    }
    std::string queryParams;
    for (const auto& [fst, snd] : params) {
        queryParams += "&" + fst + "=" + snd;
    }
    queryParams = queryParams.substr(1);
    return queryParams;
}

std::string yijinc::BeastHttp::request(
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::string& body,
    const http::verb& method) {
    // 所有参数定义出来
    net::io_context ioc;
    tcp::resolver resolver(ioc);
    std::string port;
    // 如果是https就使用ssl_stream，否则就是tcp_stream
    http::response<http::string_body> res;
    std::string res_string;
    try {
        // 解析URL
        if (DEBUG_ENABLE) {
            logger::info("url: {}", url);
        }
        urls::url_view parsed_url(url);
        std::string host = std::string(parsed_url.host());
        std::string target =
            std::string(parsed_url.encoded_path()).empty() ? "/" : std::string(parsed_url.encoded_path());
        std::string params = std::string(parsed_url.encoded_query());
        std::string scheme = std::string(parsed_url.scheme());
        if (params.size() > 0) {
            target += "?" + params;
        }
        if (DEBUG_ENABLE) {
            logger::info("scheme: {}, host: {}, target: {}", scheme, host, target);
        }
        // 设置HTTP请求
        http::request<http::string_body> req { method, target, 11 };
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        // 设置headers
        if (headers.size() > 0) {
            for (const auto& header : headers) {
                req.set(header.first, header.second);
                if (DEBUG_ENABLE) {
                    logger::info("set header: {} => {}", header.first, header.second);
                }
            }
        }
        if (body.length() > 0) {
            req.body() = body;
            req.prepare_payload();
        }
        if (scheme == "https") {
            res = https_request(ioc, method, req);
        } else {
            res = http_request(ioc, method, req);
        }
        if (DEBUG_ENABLE) {
            logger::info("result code: {}", res.result_int());
        }
        if (res.result() == http::status::ok) {
            logger::info("HTTP OK");
        } else if (res.result() == http::status::found) {
            logger::warn("HTTP 302 NOT FOUND");
            // 处理重定向问题
            auto location = res.find(http::field::location);
            if (location != res.end()) {
                std::string redirect_url = std::string(location->value());
                logger::info("redirect url: {}", redirect_url);
                return request(redirect_url, headers, body, method);
            } else {
                logger::error("redirect url not found");
            }
        }
        return res.body().data();
    } catch (const std::exception& e) {
        logger::error("HTTP error: {}", e.what());
        // 读取响应体
        return res_string;
    }
}

http::response<http::string_body> yijinc::BeastHttp::http_request(
    net::io_context& ioc,
    const http::verb& method,
    http::request<http::string_body>& req) {
    http::response<http::string_body> res;
    beast::tcp_stream stream(ioc);
    std::string target_host = std::string(req[http::field::host]);
    std::string target_port = "80";
    tcp::resolver resolver(ioc);
    // 接收HTTP响应
    beast::flat_buffer buffer;
    auto const results = resolver.resolve(target_host, target_port);
    if (proxy_enable) {
        // 解析代理服务器
        beast::tcp_stream proxy_stream(ioc);
        logger::info("proxy host: {}, proxy port: {}", proxy_host, proxy_port);
        tcp::resolver::query proxy_query(proxy_host, proxy_port);
        auto const proxy_results = resolver.resolve(proxy_query);
        proxy_stream.connect(proxy_results);
        // 发送 CONNECT 连接请求
        http::request<http::empty_body> connect_req { http::verb::connect, target_host + ":" + target_port, 11 };
        connect_req.set(http::field::host, target_host);
        connect_req.set(http::field::proxy_connection, "Keep-Alive");
        connect_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(proxy_stream, connect_req);
        http::response<http::empty_body> connect_res;
        // 读取 CONNECT 响应
        http::parser<false, http::empty_body> p;
        p.skip(true);
        http::read(proxy_stream, buffer, p);
    } else {
        // 同步连接
        net::connect(stream.socket(), results.begin(), results.end());
        // 发送请求
        http::write(stream, req);
        // 读取响应
        http::read(stream, buffer, res);
    }
    logger::info("close stream start.");
    close_stream(stream);
    return res;
}

http::response<http::string_body> yijinc::BeastHttp::https_request(
    net::io_context& ioc,
    const http::verb& method,
    http::request<http::string_body>& req) {
    // net::steady_timer timer(ioc);
    beast::flat_buffer buffer;
    std::string target_host = std::string(req[http::field::host]);
    std::string target_port = "443";
    logger::info("target_host: {}, target_port: {}", target_host, target_port);
    tcp::resolver resolver(ioc);
    net::ssl::context ctx(net::ssl::context::sslv23_client);
    // 设置验证模式
    // ctx.set_verify_mode(net::ssl::verify_none);
    ctx.set_verify_mode(net::ssl::verify_peer);
    // 加载系统默认的信任证书
    ctx.set_default_verify_paths();
    boost::optional<beast::ssl_stream<beast::tcp_stream>> ssl_stream;
    // 如果开启代理，那么就使用代理
    if (proxy_enable) {
        // 解析代理服务器
        beast::tcp_stream proxy_stream(ioc);
        logger::info("proxy host: {}, proxy port: {}", proxy_host, proxy_port);
        auto const& proxy_results = resolver.resolve(proxy_host, proxy_port);
        proxy_stream.connect(proxy_results);
        // 发送 CONNECT 连接请求
        http::request<http::string_body> connect_req { http::verb::connect, target_host + ":" + target_port, 11 };
        connect_req.set(http::field::host, target_host + ":" + target_port);
        connect_req.set(http::field::proxy_connection, "Keep-Alive");
        connect_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.set(http::field::user_agent, "Boost.Beast");

        http::write(proxy_stream, connect_req);
        http::response<http::string_body> connect_res;
        // 读取 CONNECT 响应
        http::parser<false, http::string_body> p(connect_res);
        p.skip(true);
        http::read(proxy_stream, buffer, p);
        // 检查响应状态
        if (DEBUG_ENABLE) {
            logger::info("connect res: {}", connect_res.result_int());
        }
        ssl_stream.emplace(std::move(proxy_stream), ctx);
    } else {
        ssl_stream.emplace(ioc, ctx);
        auto const results = resolver.resolve(target_host, target_port);
        auto& ssl_stream_ref = *ssl_stream;
        net::connect(beast::get_lowest_layer(ssl_stream_ref).socket(), results.begin(), results.end());
    }
    // 不用延迟等待
    beast::get_lowest_layer(*ssl_stream).socket().set_option(boost::asio::ip::tcp::no_delay(true));

    // 握手
    if (!SSL_set_tlsext_host_name(ssl_stream->native_handle(), target_host.c_str())) {
        beast::error_code ec { static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };
        throw beast::system_error { ec };
    }
    //  设置 SNI (Server Name Indication)
    if (DEBUG_ENABLE) {
        logger::info("set sni target_host: {}", target_host);
    }
    // 握手
    ssl_stream->handshake(net::ssl::stream_base::client);
    if (DEBUG_ENABLE) {
        logger::info("Stream handshake success.");
    }
    http::response<http::string_body> res;
    // 发送请求
    http::write(*ssl_stream, req);
    if (DEBUG_ENABLE) {
        logger::info("http request success.");
    }
    // 读取响应
    http::read(*ssl_stream, buffer, res);
    close_stream(*ssl_stream);
    return res;
}

// 开启代理
void yijinc::BeastHttp::set_proxy(const std::string& host, const std::string& port) {
    proxy_host = host;
    proxy_port = port;
    proxy_enable = true;
}

void yijinc::BeastHttp::close_stream(boost::beast::ssl_stream<boost::beast::tcp_stream>& ssl_stream) {
    boost::system::error_code ec;
    // 取消所有挂起的操作
    ssl_stream.next_layer().cancel();
    if (ec) {
        logger::error("Cancel error: {}", ec.message());
    }
    // 同步关闭 SSL 连接
    ssl_stream.shutdown(ec);
    if (ec && ec != boost::asio::ssl::error::stream_truncated) {
        logger::error("SSL shutdown error: {}", ec.message());
    }
    // 检查套接字是否已连接
    if (ssl_stream.next_layer().socket().is_open()) {
        // 关闭底层 TCP 套接字
        ssl_stream.next_layer().socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        if (ec) {
            logger::error("ssl stream shutdown error: {}", ec.message());
        }
        ssl_stream.next_layer().close();
    } else {
        logger::warn("Socket is already closed.");
    }
}

void yijinc::BeastHttp::close_stream(boost::beast::tcp_stream& tcp_stream) {
    boost::system::error_code ec;
    // 取消所有挂起的操作
    tcp_stream.cancel();
    if (ec) {
        logger::error("Cancel error: {}", ec.message());
    }
    // 检查套接字是否已连接
    if (tcp_stream.socket().is_open()) {
        // 关闭底层 TCP 套接字
        tcp_stream.socket().shutdown(tcp::socket::shutdown_both, ec);
        if (ec) {
            logger::error("ssl stream shutdown error: {}", ec.message());
        }
        tcp_stream.close();
    } else {
        logger::warn("Socket is already closed.");
    }
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string yijinc::BeastHttp::curl_get(
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& params) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return ""; // 初始化失败，直接返回
    }
    std::string full_url = url;
    if (params.size() > 0) {
        std::string params_str = to_query_params(params);
        full_url += "?" + params_str;
    }
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    if (!headers.empty()) {
        struct curl_slist* headerlist = nullptr; // 初始化为空的链表
        // 将每个请求头添加到链表中
        for (const auto& header : headers) {
            std::string header_string = header.first + ": " + header.second;
            headerlist = curl_slist_append(headerlist, header_string.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    }
    if (proxy_enable) {
        curl_easy_setopt(curl, CURLOPT_PROXY, std::string("http://" + proxy_host + ":" + proxy_port).c_str());
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    return response;
}

std::string yijinc::BeastHttp::curl_post(
    const std::string& url,
    const std::map<std::string, std::string>& headers,
    const std::string& body) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return ""; // 初始化失败，返回空字符串
    }
    std::string response;
    struct curl_slist* headerlist = nullptr;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // 设置目标 URL
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,
                     body.c_str());                   // 设置 POST 请求体

    if (proxy_enable) {
        std::string proxy_str = "http://" + proxy_host + ":" + proxy_port;
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy_str.c_str());
    }

    // 设置请求头
    for (const auto& header : headers) {
        std::string header_string = header.first + ": " + header.second;
        headerlist = curl_slist_append(headerlist, header_string.c_str());
    }
    if (headerlist) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    }

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    if (curl == NULL) {
        std::cerr << "curl pointer is NULL" << std::endl;
        return "";
    }
    CURLcode res = curl_easy_perform(curl); // 执行请求
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << '\n';
    }

    curl_easy_cleanup(curl);
    if (headerlist) {
        curl_slist_free_all(headerlist); // 释放请求头链表
    }

    return response; // 返回响应字符串
}
