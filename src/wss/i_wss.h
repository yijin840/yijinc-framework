#ifndef WSS_HPP
#define WSS_HPP
#include <iostream>
#include <string>
#include <vector>

#include "i_wss_context.h"

namespace yijinc {
class IWss {
public:
    virtual ~IWss() = delete;

    // 连接
    virtual std::string connect() = 0;

    virtual std::string onConnect() = 0;

    // 心跳
    virtual std::string ping() = 0;

    virtual std::string pong() = 0;

    virtual std::string onPing() = 0;

    virtual std::string onPong() = 0;

    // 异常
    virtual std::string onError() = 0;

    // 订阅
    virtual std::string subscribe() = 0;

    virtual std::string unSubscribe() = 0;

    // 信息
    virtual std::string sendMessage() = 0;

    virtual std::string readMessage() = 0;

    // 异步
    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void asyncSendMessage() = 0;

    virtual void asyncReadMessage() = 0;

    // 服务
    virtual void startServer() = 0;

    virtual void restartServer() = 0;

    virtual void stopServer() = 0;

protected:
    std::vector<std::string> subscriptTopic;
    std::shared_ptr<IWssContext> wssContextPtr;
};
} // namespace yijinc

#endif
