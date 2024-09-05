//
// Created by yijin on 2024-09-05 at 11:56:00.
//
#ifndef BEAST_WSS_CONTEXT_H
#define BEAST_WSS_CONTEXT_H
#include <memory>

#include "i_wss_context.h"

namespace yijinc {
    class beast_wss_context : public std::shared_ptr<IWssContext> {
    public:
        ~beast_wss_context() = delete;

    };
}


#endif //BEAST_WSS_CONTEXT_H
