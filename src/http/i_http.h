#ifndef I_HTTP_HPP
#define I_HTTP_HPP

#include <iostream>
#include <map>
#include <string>

namespace yijinc {
    class IHttp {
    public:
        enum class ClientType {
            CURL,
            BOOST
        };

        virtual ~IHttp() = default;

        explicit IHttp(ClientType type = ClientType::CURL) : client_type(type) {
        }

        /**
         * @brief
         *  get实现
         * @param url
         * @param headers
         * @param params
         * @return std::string
         */
        virtual std::string
        get(const std::string &url,
            std::map<std::string, std::string> &headers,
            const std::map<std::string, std::string> &params);

        /**
         * @brief
         * post实现
         * @param url
         * @param body
         * @param headers
         * @param body
         * @return std::string
         */
        virtual std::string
        post(const std::string &url, const std::string &body, const std::map<std::string, std::string> &headers);

    protected:
        ClientType client_type = ClientType::CURL; // 默认的是curl
    };
}

#endif
