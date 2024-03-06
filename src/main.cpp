#include <stdio.h> 
#include "tcpSocket.h"
#include <cstring>
#include <chrono>
#include <ctime>
#include "storage.hpp"
#include "vehicle.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include <iostream>

#define PORT 8888
#define VERSION "v240306\r\n"

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
    int ret = -1;
    std::stringstream ss;
    ss << recv;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    boost::property_tree::ptree j_sensor = pt.get_child("sensor");

    if (j_sensor.get_value<std::string>() != "gps") {
        return -1;
    }
    // uuid
    boost::uuids::string_generator uuid_gen;
    boost::property_tree::ptree j_uuid = pt.get_child("uuid");
    boost::uuids::uuid uuid = uuid_gen(j_uuid.get_value<std::string>());

    // time
    boost::property_tree::ptree j_date = pt.get_child("date");
    boost::property_tree::ptree j_time = pt.get_child("time");
    uint64_t r_time = j_date.get_value<uint64_t>();
    r_time *= 1000000000;
    r_time += j_time.get_value<uint64_t>();

    // position
    double position[3];
    unsigned int i = 0;
    for (auto& item : pt.get_child("position"))
        position[i++] = item.second.get_value<double>();
    
    // position std_dev
    double pos_std_dev[3];
    i = 0;
    for (auto& item : pt.get_child("std_dev"))
        pos_std_dev[i++] = item.second.get_value<double>();
    
    // parse json msg
    Vehicle& av = stor.get(uuid);
    av.setPosition(position[LAT], position[LON], position[ALT], r_time);
    av.setPosStdDev(pos_std_dev[LAT],pos_std_dev[LON],pos_std_dev[ALT]);

    // answer with list of all avs
    int idx = stor.toCString(send, TCPSOCKET_SENDBUFFERSIZE, av);
    if (idx > 0 && idx + 2 < TCPSOCKET_SENDBUFFERSIZE)
        std::memcpy(send + idx - 1,"\r\n",3); // (-1) overwrite terminate char
        return idx + 2;
    
    std::memcpy(send, "ERR: send buffer overflow\r\n", 28);
    return ret;
}

/* Generate a json-message over vehicles in storage */
int show(char* recv, char* send) {
    int idx = stor.toCString(send, TCPSOCKET_SENDBUFFERSIZE);
    if (idx > 0 && idx + 2 < TCPSOCKET_SENDBUFFERSIZE)
        std::memcpy(send + idx - 1,"\r\n",3); // (-1) overwrite terminate char
        return idx + 2;
    
    std::memcpy(send, "ERR: send buffer overflow\r\n", 28);
}

int main(int argc , char *argv[]) {
    char helo_msg[] = "HELO - AVeStDi daemon at " VERSION;
    TcpSocket tsck(PORT, helo_msg);
    tsck.init();
    tsck.addCmd("VERS", vers);
    tsck.addCmd("QUIT", quit);
    tsck.addCmd("ECHO", echo);
    tsck.addCmd("PUSH", push);
    tsck.addCmd("SHOW", show);
    while(1) {
        tsck.spin();
    }
    return 0;
}

//PUSH {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603009","date":"20220227","time":"014307692","position":[53.1217843,8.7033962,8.4],"std_dev":[0.98,1.70,2.10],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12};
//PUSH {"sensor":"gps","uuid":"0f389c46-ea13-45e0-b6a7-af282a603008","date":"20220227","time":"014307699","position":[54.1217843,8.2033962,8.4],"std_dev":[0.98,1.70,2.10],"velocity":0.055,"course":0.000,"mode":5,"sat_cnt":12}