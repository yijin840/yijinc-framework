//
// Created by yijin on 2024-09-05 at 11:56:00.
//
#ifndef BEAST_WSS_CONTEXT_H
#define BEAST_WSS_CONTEXT_H
#include <memory>

#include "IWssContext.h"

namespace yijinc {
class BeastWssContext : public std::shared_ptr<IWssContext> {
public:
    ~BeastWssContext() = delete;
};
}

#endif // BEAST_WSS_CONTEXT_H
