//
// Created by yijin on 2024-09-04 at 16:30:30.
//
#ifndef YIJINC_EXCEPTION_H
#define YIJINC_EXCEPTION_H
#include <locale>
#include <boost/exception/exception.hpp>
#include <utility>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>
#include <execinfo.h>
#include <cxxabi.h>
#include <boost/stacktrace.hpp>

class YijincException final : public std::exception {
public:
    explicit YijincException(const std::string &message, std::string code) : message(message), code(std::move(code)) {
    }

    explicit YijincException(const std::string &message) : message(message) {
    }

    [[nodiscard]] static std::string getOffsetTrace() {
        void *callstack[128];
        const int frames = backtrace(callstack, 128);
        char **symbols = backtrace_symbols(callstack, frames);

        std::ostringstream oss;
        oss << "Stack trace:" << std::endl;
        for (int i = 1; i < frames; ++i) {
            int status;
            char *demangled = abi::__cxa_demangle(symbols[i], nullptr, nullptr, &status);
            if (status == 0) {
                oss << "    " << demangled << std::endl;
                free(demangled);
            } else {
                oss << "    " << symbols[i] << std::endl;
            }
        }

        free(symbols);
        return oss.str();
    }

    [[nodiscard]] static std::string getStackTrace() {
        const auto frames = boost::stacktrace::stacktrace();
        std::ostringstream oss;
        oss << "Stack trace:" << std::endl;
        for (auto frame: frames) {
            oss << "    " << frame << std::endl;
        }
        return oss.str();
    }

    [[nodiscard]] std::string getMessage() const {
        return this->message;
    }

private:
    const std::string &message;
    std::string code;
    std::string stack;
};


#endif //YIJINC_EXCEPTION_H
