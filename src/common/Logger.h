#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "log4cplus/consoleappender.h"
#include "log4cplus/logger.h"
#include "log4cplus/loglevel.h"
#include "string"
#include <fmt/core.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

// 初始化日志系统
#define LOG_INIT(config_file) log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(config_file))

#define FILE_NAME std::string(__BASE_FILE__)
// 获取日志实例
#define GET_LOGGER() log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(__FUNCTION__))

// 封装日志记录
#define LOG_INFO(fmt_str, ...) LOG4CPLUS_INFO(GET_LOGGER(), fmt::format(fmt_str, ##__VA_ARGS__))

#define LOG_DEBUG(fmt_str, ...) LOG4CPLUS_DEBUG(GET_LOGGER(), fmt::format(fmt_str, ##__VA_ARGS__))

#define LOG_WARN(fmt_str, ...) LOG4CPLUS_WARN(GET_LOGGER(), fmt::format(fmt_str, ##__VA_ARGS__))

#define LOG_ERROR(fmt_str, ...) LOG4CPLUS_ERROR(GET_LOGGER(), fmt::format(fmt_str, ##__VA_ARGS__))

#endif
