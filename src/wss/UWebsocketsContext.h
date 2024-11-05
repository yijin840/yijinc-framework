// //
// // Created by yijin on 2024-10-30 at 11:55:39.
// //
// #ifndef UWEBSOCKETS_H
// #define UWEBSOCKETS_H
// #include <uWebSockets/App.h>
// #include <uWebSockets/WebSocket.h>
// #include <uWebsockets/ClientApp.h>

// #define WS_URL "ws://127.0.0.1:8080"

// class UWebsocketsContext {
// public:
//     struct PerSocketData {};

//     UWebsocketsContext() = default;
//     ~UWebsocketsContext() = default;
//     void connect();
//     void initApp();

// private:
//     std::shared_ptr<uWS::App> app;
//     // uWS::App app;
//     std::shared_ptr<uWS::ClientApp> client;
//     std::shared_ptr<uWS::App::WebSocketBehavior<PerSocketData>> behavior;
//     std::shared_ptr<uWS::WebSocket<false, true, PerSocketData>> ws;
// };

// #endif // UWEBSOCKETS_H
