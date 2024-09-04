#include "common/logger.h"
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using logger = yijinc::Logger;

int main(int argc, char *argv[]) {
    // 示例日志记录
    logger::initLogger(argc, argv);
    // http_client::init_ssl();
    // http_client::set_proxy("127.0.0.1", "7897");
    logger::info("test log format info ==> {}", "hello world");
    std::fstream f("test.json");
    if (f.is_open()) {
        logger::info("file is opened");
    }
    json data = json::parse(f);
    logger::info("json pi ==> {}", data["pi"].dump());
    //file close
    f.close();
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    // const std::string &&resp = http::post("http://www.baidu.com", headers,
    // ""); logger::info("resp => {}", resp);
    return 0;
}
