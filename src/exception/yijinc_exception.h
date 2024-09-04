//
// Created by yijin on 2024-09-04 at 16:30:30.
//
#ifndef YIJINC_EXCEPTION_H
#define YIJINC_EXCEPTION_H
#include <locale>
#include <boost/exception/exception.hpp>


class YijincException final : public std::exception {
public:
    const std::string &message;
    const std::string &code;
    std::string stack;

    explicit YijincException(const std::string &message, const std::string &code) : message(message), code(code) {
    }

    [[nodiscard]] std::string getExceptionStack() const {
        return stack;
    }
};


#endif //YIJINC_EXCEPTION_H
