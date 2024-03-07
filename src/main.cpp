#include <stdio.h> 
#include "tcpSocket.h"

#include "storage.hpp"
#include "vehicle.hpp"
#include "cmdHandler.hpp"

#include <iostream>
#include "App.h"
#include <thread>
#include <algorithm>
#include <libusockets.h>

#define PORT 8888
#define WS_BUFFER_SIZE 16384

int main(int argc , char *argv[]) {
        struct PerSocketData {

    };

    /* Simple echo websocket server, using multiple threads */
    std::vector<std::thread *> threads(std::thread::hardware_concurrency());

    std::transform(threads.begin(), threads.end(), threads.begin(), [](std::thread */*t*/) {
        return new std::thread([]() {

            /* Very simple WebSocket echo server */
            char r_buffer[WS_BUFFER_SIZE];
            char s_buffer[WS_BUFFER_SIZE];
            uWS::App().ws<PerSocketData>("/*", {
                /* Settings */
                .compression = uWS::SHARED_COMPRESSOR,
                .maxPayloadLength = 16 * 1024,
                .idleTimeout = 10,
                .maxBackpressure = 1 * 1024 * 1024,
                /* Handlers */
                .upgrade = nullptr,
                .open = [](auto */*ws*/) {

                },
                .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
                    ws->send(message, opCode);
                },
                .drain = [](auto */*ws*/) {
                    /* Check getBufferedAmount here */
                },
                .ping = [](auto */*ws*/, std::string_view) {

                },
                .pong = [](auto */*ws*/, std::string_view) {

                },
                .close = [](auto */*ws*/, int /*code*/, std::string_view /*message*/) {

                }
            }).get("/version", [&s_buffer](auto *res, auto * /*req*/) {
                CmdHandler::vers(NULL,s_buffer);
                res->end(s_buffer);
            }).get("/*", [&s_buffer](auto *res, auto * /*req*/) {
                CmdHandler::show(NULL,s_buffer);
                res->end(s_buffer);
            }).listen(9001, [](auto *listen_socket) {
                if (listen_socket) {
                    std::cout << "Thread " << std::this_thread::get_id() << " listening on port " << 9001 << std::endl;
                } else {
                    std::cout << "Thread " << std::this_thread::get_id() << " failed to listen on port 9001" << std::endl;
                }
            }).run();

        });
    });

    char helo_msg[] = "HELO - AVeStDi daemon at " VERSION;
    TcpSocket tsck(PORT, helo_msg);
    tsck.init();
    tsck.addCmd("VERS", CmdHandler::vers);
    tsck.addCmd("QUIT", CmdHandler::quit);
    tsck.addCmd("PUTL", CmdHandler::putl);
    tsck.addCmd("PUTS", CmdHandler::puts);
    tsck.addCmd("SHOW", CmdHandler::show);
    while(1) {
        tsck.spin();
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread *t) {
        t->join();
    });

    return 0;
}

//PUTL {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603009","date":"20220227","time":"014307692","position":[53.1217843,8.7033962,8.4],"std_dev":[0.98,1.70,2.10],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12};
//PUTS {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603008","date":"20220227","time":"014307699","position":[54.1217843,8.2033962,8.4],"std_dev":[0.98,1.70,2.10],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12}