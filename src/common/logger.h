#ifndef LOGGER_HPP
#define LOGGER_HPP

// 初始化日志系统
#define LOG_INIT() \
log4cplus::Initializer initializer; \
log4cplus::BasicConfigurator config; \
config.configure();

#include "log4cplus/consoleappender.h"
#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include <fmt/core.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace yijinc {
    // 自定义命名空间

    class Logger {
    public:
        static void initLogger(int argc, char *argv[]) {
            log4cplus::initialize();
            log4cplus::PropertyConfigurator::doConfigure("log4cplus.properties");
        }

        static log4cplus::Logger &getLogger() {
            static log4cplus::Logger logger = log4cplus::Logger::getRoot();
            return logger;
        }

        template<typename T, typename... Args>
        static void info(const T &message, Args &&... args) {
            if (auto &logger = getLogger(); logger.isEnabledFor(log4cplus::INFO_LOG_LEVEL)) {
                const std::string formatted_message =
                        fmt::format(message, std::forward<Args>(args)...);
                LOG4CPLUS_INFO(logger, formatted_message);
            }
        }

        template<typename T, typename... Args>
        static void warn(const T &message, Args &&... args) {
            if (auto &logger = getLogger(); logger.isEnabledFor(log4cplus::WARN_LOG_LEVEL)) {
                const std::string formatted_message =
                        fmt::format(message, std::forward<Args>(args)...);
                LOG4CPLUS_WARN(logger, formatted_message);
            }
        }

        template<typename T, typename... Args>
        static void error(const T &message, Args &&... args) {
            if (auto &logger = getLogger(); logger.isEnabledFor(log4cplus::ERROR_LOG_LEVEL)) {
                const std::string formatted_message =
                        fmt::format(message, std::forward<Args>(args)...);
                LOG4CPLUS_ERROR(logger, formatted_message);
            }
        }
    };
} // namespace yijinc
#endif // LOGGER_HPP
