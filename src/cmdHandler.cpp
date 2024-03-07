#include "cmdHandler.hpp"
#include "tcpSocket.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


Storage CmdHandler::_stor = Storage();

int CmdHandler::vers(char*, char* send) {
    const char msg[] = VERSION;
    std::memcpy(send,msg,10);
    return 14;
}

int CmdHandler::quit(char*, char* send) {
    const char msg[] = "EXIT bye\r\n";
    std::memcpy(send, msg, 11);
    return -2;
}

Vehicle& CmdHandler::updateAVbyJson(char* recv) {
    std::stringstream ss;
    ss << recv;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    boost::property_tree::ptree j_sensor = pt.get_child("sensor");

    if (j_sensor.get_value<std::string>() != "gps") {
        throw std::invalid_argument("JSON sting does not contain gps sensor data");
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
    Vehicle& av = _stor.get(uuid);
    av.setPosition(position[LAT], position[LON], position[ALT], r_time);
    av.setPosStdDev(pos_std_dev[LAT],pos_std_dev[LON],pos_std_dev[ALT]);
    return av;
}

int CmdHandler::puts(char* recv, char* send) {
    try {
        Vehicle& av = updateAVbyJson(recv);
        std::memcpy(send, "OK\r\n", 5);
        return 5;
    } catch (const std::invalid_argument& e) {
        std::memcpy(send, "ERR: no gps data found in JSON string\r\n", 40);
        return 40;
    } catch (const boost::property_tree::ptree_bad_data& e) {
        std::memcpy(send, "ERR: incomplete gps data in json string\r\n", 42);
        return 42;
    } catch (const boost::property_tree::json_parser_error& e) {
        std::memcpy(send, "ERR: json format error, cannot parse\r\n", 39);
        return 39;
    }
}

int CmdHandler::putl(char* recv, char* send) {
    try {
        Vehicle& av = updateAVbyJson(recv);
        // answer with list of all avs
        int idx = _stor.toCString(send, TCPSOCKET_SENDBUFFERSIZE, av);
        if (idx > 0 && idx + 2 < TCPSOCKET_SENDBUFFERSIZE) {
            std::memcpy(send + idx - 1,"\r\n",3); // (-1) overwrite terminate char
            return idx + 2;
        }
    } catch (const std::invalid_argument& e) {
        std::memcpy(send, "ERR: no gps data found in JSON string\r\n", 40);
    } catch (const boost::property_tree::ptree_bad_data& e) {
        std::memcpy(send, "ERR: incomplete gps data in json string\r\n", 42);
    } catch (const boost::property_tree::json_parser_error& e) {
        std::memcpy(send, "ERR: json format error, cannot parse\r\n", 39);
    }

    std::memcpy(send, "ERR: send buffer overflow\r\n", 28);
    return 28;
}

/* Generate a json-message over vehicles in storage */
int CmdHandler::show(char*, char* send) {
    int idx = _stor.toCString(send, TCPSOCKET_SENDBUFFERSIZE);
    if (idx > 0 && idx + 2 < TCPSOCKET_SENDBUFFERSIZE) {
        std::memcpy(send + idx - 1,"\r\n",3); // (-1) overwrite terminate char
        return idx + 2;
    }
    std::memcpy(send, "ERR: send buffer overflow\r\n", 28);
    return 28;
}
