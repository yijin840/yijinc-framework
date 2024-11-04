//
// Created by yijin on 2024-10-30 at 11:55:39.
//
#include "UWebsocketsContext.h"

#include <asio/connect.hpp>
#include <openssl/ssl.h>

using namespace std;

void UWebsocketsContext::connect() {
    ws = std::make_shared<uWS::WebSocket<false, true, PerSocketData>>();
    // ws->connect("ws://127.0.0.1:8080");
}

void UWebsocketsContext::initApp() {
    app = std::make_shared<uWS::App>(uWS::App());
    behavior =
        std::make_shared<uWS::App::WebSocketBehavior<PerSocketData>>(uWS::App::WebSocketBehavior<PerSocketData>());
    behavior->open = [](auto* client) {
        cout << "onOpen" << endl;
    };
    behavior->close = [](auto* client, int code, std::string_view reason) {
        cout << "close: " + std::string(reason) << endl;
    };
    behavior->message = [](auto* client, std::string_view message, uWS::OpCode opCode) {
        cout << "message: " + std::string(message) << endl;
    };
    behavior->dropped = [](auto* client, std::string_view message, uWS::OpCode opCode) {
        cout << "Connection dropped" << endl;
    };
    behavior->drain = [](auto* client) {
        cout << "Drain event" << endl;
    };
    const auto& cachingApp = app->ws("/*", std::move(*behavior));
    cout << "initApp" << endl;
}
