#include <stdio.h> 
#include "tcpSocket.h"
#include <cstring>
#include <chrono>
#include <ctime>
#include "storage.hpp"
#include "vehicle.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define PORT 8888
#define VERSION "v240221\r\n"

Storage stor = Storage();

int vers(char* recv, char* send) {
    const char msg[] = VERSION;
    std::memcpy(send,msg,10);
    return 14;
}

int quit(char* recv, char* send) {
    const char msg[] = "EXIT bye\r\n";
    std::memcpy(send, msg, 11);
    return -2;
}

int echo(char* recv, char* send) {
    size_t msg_len = strlen(recv);
    std::memcpy(send,recv,msg_len);
    return msg_len;
}

int push(char* recv, char* send) {
    std::string str = "{\"sensor\":\"gps\",\"id\":1,\"time\":214307692,\"position\":[53.1217843,8.2033962,8.4],\"std_dev\":[0.98,1.70,2.10],\"velocity\":0.055,\"course\":0.000,\"mode\":5,\"sat_cnt\":12}";
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(str, pt);
    boost::property_tree::basic_ptree<std::string,std::string>::const_iterator iter = pt.begin(),iterEnd = pt.end();
    __uint128_t uuid = 0;

    for(;iter != iterEnd;++iter) {
        iter->first;
        iter->second;
    }
    double lat;
    double lon;
    double alt;
    time_t timestamp = std::time(0);
    // parse json msg
    if (stor.contains(uuid)) {
        Vehicle& av = stor.get(uuid);
        av.setPosition(lat, lon, alt, timestamp);
    } else {
        stor.put(uuid, lat, lon, alt, timestamp);
    }
    return 0;
}

int main(int argc , char *argv[]) {
    char helo_msg[] = "HELO - AVeStDi daemon at " VERSION;
    TcpSocket tsck(PORT, helo_msg);
    tsck.init();
    tsck.addCmd("VERS", vers);
    tsck.addCmd("QUIT", quit);
    tsck.addCmd("ECHO", echo);
    tsck.addCmd("PUSH", push);
    while(1) {
        tsck.spin();
    }
    return 0;
}