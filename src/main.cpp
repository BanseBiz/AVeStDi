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

#define TCP_PORT 6156
#define WS_PORT 8096
#define WS_BUFFER_SIZE 16384

int main(int argc , char *argv[]) {
        struct PerSocketData {

    };
    std::vector<std::thread *> threads(std::thread::hardware_concurrency());

    std::transform(threads.begin(), threads.end(), threads.begin(), [](std::thread */*t*/) {
        return new std::thread([]() {
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
            }).get("/*", [&s_buffer](auto *res, auto * /*req*/) {
                int idx = CmdHandler::vers(NULL,s_buffer);
                s_buffer[idx] = '\0';
                res->end(s_buffer);
            }).get("/list", [&s_buffer](auto *res, auto * /*req*/) {
                CmdHandler::show(NULL,s_buffer);
                res->end(s_buffer);
            }).put("/list", [&s_buffer, &r_buffer](auto *res, auto *req){
                std::string buffer;
                res->onAborted([=]() {
                    std::cout << "HTTP socket was closed before we upgraded it!" << std::endl;
                });
                res->onData([res, s_buffer, r_buffer](std::string_view data, bool last) mutable {
                    if (data.length() > WS_BUFFER_SIZE) {
                        res->end("ERR: received msg is to large");
                    } else {
                        std::memcpy(r_buffer, data.data(), data.length());
                        if (last) {
                            int idx = CmdHandler::putl(r_buffer,s_buffer);
                            r_buffer[idx] = '\0';
                            res->end(s_buffer);
                        }
                    }
                });
            }).put("/*", [&s_buffer, &r_buffer](auto *res, auto *req){
                std::string buffer;
                res->onAborted([=]() {
                    std::cout << "HTTP socket was closed before we upgraded it!" << std::endl;
                });
                res->onData([res, s_buffer, r_buffer](std::string_view data, bool last) mutable {
                    if (data.length() > WS_BUFFER_SIZE) {
                        res->end("ERR: received msg is to large");
                    } else {
                        std::memcpy(r_buffer, data.data(), data.length());
                        if (last) {
                            int idx = CmdHandler::puts(r_buffer,s_buffer);
                            r_buffer[idx] = '\0';
                            res->end(s_buffer);
                        }
                    }
                });
            }).listen(WS_PORT, [](auto *listen_socket) {
                if (listen_socket) {
                    std::cout << "Thread " << std::this_thread::get_id() << " listening on TCP_PORT " << WS_PORT << std::endl;
                } else {
                    std::cout << "Thread " << std::this_thread::get_id() << " failed to listen on TCP_PORT " << WS_PORT << std::endl;
                }
            }).run();

        });
    });

    char helo_msg[] = "HELO - AVeStDi daemon"; //AutonomousVehicleStatusDistributer
    TcpSocket tsck(TCP_PORT, helo_msg);
    tsck.init();
    tsck.addCmd("VERS", CmdHandler::vers);
    tsck.addCmd("QUIT", CmdHandler::quit);
    tsck.addCmd("PUTL", CmdHandler::putl);
    tsck.addCmd("PUTS", CmdHandler::puts);
    tsck.addCmd("LIST", CmdHandler::show);
    while(1) {
        tsck.spin();
    }

    std::for_each(threads.begin(), threads.end(), [](std::thread *t) {
        t->join();
    });

    return 0;
}