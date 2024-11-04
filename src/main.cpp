#include "common/Logger.h"
#include "exception/YijincException.h"
#include "wss/UWebsocketsContext.h"

#include <boost/any.hpp> // generators
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/beast/core/detail/bind_handler.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <crow.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <opencv/OpencvApp.h>
#include <string>
#include <utility>
#include <uuid/uuid.h>
#include <uwebsockets/App.h>

using json = nlohmann::json;
using json = nlohmann::json;
using namespace std;

struct Model {
    std::string id;
    std::string name;
    std::string path;
    std::string type;

    // 自定义构造函数
    Model(std::string id, const std::string& name, const std::string& path, const std::string& type) :
        id(std::move(id)),
        name(name),
        path(path),
        type(type) {}
};

std::mutex modelMutex; // 定义互斥锁
boost::asio::io_context ioc;
std::unique_ptr<boost::asio::steady_timer> timer;

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
    } catch (const YijincException& e) {
        LOG_ERROR("YijincException: {}", e.getOffsetTrace());
        throw YijincException("aaa");
    }
}

void init() {
    LOG_INIT("log4cplus.properties"); // 初始化日志系统
    // testLog();
    // LOG_INFO("test log format info ==> {}", "hello world");
    // std::fstream f("test.json");
    // if (f.is_open()) {
    //     LOG_INFO("file is opened");
    // }
    // json data = json::parse(f);
    // LOG_INFO("json pi ==> {}", data["pi"].dump());
    // // file close
    // f.close();
    // try {
    //     testError();
    // } catch (const std::exception& e) {
    //     LOG_ERROR("std::exception: {}", e.what());
    // }
    // LOG_INFO("test log format info ==> {}", "hello world");
}

int t = 0;

std::shared_ptr<std::map<std::string, std::deque<std::shared_ptr<Model>>>> historyModel =
    std::make_shared<std::map<std::string, std::deque<std::shared_ptr<Model>>>>();

string getRandom() {
    // 创建随机数引擎
    std::random_device rd;                       // 获取随机数种子
    std::mt19937 gen(rd());                      // 使用 Mersenne Twister 算法生成随机数
    std::uniform_int_distribution<> dis(0, 100); // 定义范围 [0, 5]

    // 生成并输出随机数
    const int randomNumber = dis(gen);

    return to_string(randomNumber);
}

// 测试定时任务
void testSteadyTimer() {
    timer = std::make_unique<boost::asio::steady_timer>(ioc, 0);
    timer->expires_after(std::chrono::seconds(0));
    timer->async_wait([&](const boost::system::error_code& ec) {
        if (ec) {
            cout << "test" << endl;
            cerr << ec.message() << endl;
        }
        // 随机
        const boost::uuids::uuid uuid = boost::uuids::random_generator()();
        auto uuids = to_string(uuid);
        // cout << "id: " << t << endl;
        const std::string& rs = getRandom();
        std::lock_guard lock(modelMutex); // 使用锁保护对共享数据的访问
        const auto model = std::make_shared<Model>(rs, uuids, uuids, uuids);
        if (const auto it = historyModel->find(rs); it == historyModel->end()) {
            const auto deque = std::deque { model };
            historyModel->emplace(rs, deque);
        } else {
            it->second.push_back(model);
            // 尾进头出
            if (it->second.size() > 3000) {
                it->second.pop_front();
            }
        }
        testSteadyTimer();
    });
}

void setPath(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/test").methods("GET"_method)([] {
        nlohmann::json r;
        // 获取historyModel副本
        std::lock_guard lock(modelMutex); // 使用锁保护对共享数据的访问
        const auto& historyModelCopy = *historyModel;
        for (const auto& [k, v] : historyModelCopy) {
            nlohmann::json arr;
            for (const auto& m : v) {
                nlohmann::json j;
                j["id"] = m->id;
                j["name"] = m->name;
                j["path"] = m->path;
                j["type"] = m->type;
                arr.push_back(j);
            }
            r[k] = arr;
        }
        crow::response resp(200, r.dump());
        resp.set_header("Content-Type", "application/json");
        return resp;
    });
}

void testCrow() {
    const auto app = std::make_shared<crow::SimpleApp>();
    setPath(*app);
    app->port(8001).loglevel(crow::LogLevel::Warning).multithreaded().run();
}

void testUws() {
    UWebsocketsContext context;
    context.initApp();
}

int main() {
    // testSteadyTimer();
    // std::thread t([&] { ioc.run(); });
    // t.detach();
    //     testCrow();
    //    testUws();

    // historyModel->emplace("test", std::deque { std::make_shared<Model>("1", "2", "3", "4") });
    //    auto it = historyModel->find("test")->second;
    //    auto model = it.front();
    //    it.pop_front();
    //    it.shrink_to_fit();
    //    model.reset();
    //    // std::thread([] { testUws(); }).detach();
    OpencvApp::test();
    OpencvApp::loadImg();
    const long a = std::abs(1);
    cout << "hello world" << endl;
    return 0;
}
