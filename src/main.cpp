#include "common/logger.h"
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <boost/stacktrace.hpp>
#include "exception/yijinc_exception.h"

using json = nlohmann::json;


void testLog() {
    // 使用格式化日志记录
    LOG_INFO("This is a message: {}, test => {}, float ==> {}", "aa", 1, 1.2f);
    LOG_DEBUG("Debugging with int: {}, float: {}", 42, 3.14);
    LOG_WARN("Warning with message: {}", "Be careful");
    LOG_ERROR("Error occurred with code: {}", -1);
}

void testError() {
    try {
        LOG_INFO("hello world");
        throw YijincException("test");
    } catch (const YijincException &e) {
        LOG_ERROR("YijincException: {}", e.getOffsetTrace());
        LOG_ERROR("YijincException: {}", e.getStackTrace());
        throw YijincException("aaa");
    }
}

int main(int argc, char *argv[]) {
    LOG_INIT("log4cplus.properties"); // 初始化日志系统
    testLog();

    LOG_INFO("test log format info ==> {}", "hello world");
    std::fstream f("test.json");
    if (f.is_open()) {
        LOG_INFO("file is opened");
    }
    json data = json::parse(f);
    LOG_INFO("json pi ==> {}", data["pi"].dump());
    //file close
    f.close();
    try {
        testError();
    } catch (const std::exception &e) {
        LOG_ERROR("std::exception: {}", e.what());
    }
    LOG_INFO("test log format info ==> {}", "hello world");
    return 0;
}
