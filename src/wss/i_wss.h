#ifndef WSS_HPP
#define WSS_HPP
#include <string>
#include <iostream>
#include <vector>

#include "i_wss_context.h"

namespace yijinc {
    class IWss {
    public:
        ~IWss() = delete;

        //连接
        std::string connect();

        std::string onConnect();

        //心跳
        std::string ping();

        std::string pong();

        std::string onPing();

        std::string onPong();

        //异常
        std::string onError();

        //订阅
        std::string subscribe();

        std::string unSubscribe();

        //信息
        std::string sendMessage();

        std::string readMessage();

        //异步
        void start();

        void stop();

        void asyncSendMessage(std::string message);

        void asyncReadMessage();

        //服务
        void startServer();

        void restartServer();

        void stopServer();

    protected:
        std::vector<std::string> subscriptTopic;
        std::shared_ptr<IWssContext> wss_context_;
    };
} // namespace yijinc

#endif
