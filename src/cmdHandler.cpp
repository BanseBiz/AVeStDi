#include "cmdHandler.hpp"
#include "tcpSocket.h"

#include <cstring>
#include <ctime>
#include <string>
#include <iostream>
#include <stdexcept>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


Storage CmdHandler::_stor = Storage();
std::string c_date(__DATE__);
std::string c_time(__TIME__);
std::string CmdHandler::_version("AVeSti Daemon Version: "+c_date+" "+c_time+"\r\n");

int CmdHandler::vers(char*, char* send) {
    std::memcpy(send,_version.c_str(),_version.length());
    return _version.length();
}

int CmdHandler::quit(char*, char* send) {
    const char msg[] = "EXIT bye\r\n";
    std::memcpy(send, msg, 11);
    return -2;
}

bool handleJsonOptionalValue(boost::property_tree::ptree pt, const boost::property_tree::path path, double* dest) {
    boost::optional<boost::property_tree::ptree &> json = pt.get_child_optional(path);
    if (json) {
        *dest = json.get().get_value<double>();
        return true;
    }
    return false;
}

bool handleJsonOptionalArray(boost::property_tree::ptree pt, const boost::property_tree::path path, double* dest) {
    boost::optional<boost::property_tree::ptree &> json = pt.get_child_optional(path);
    if (json) {
        unsigned int i = 0;
        for (auto& item : json.get()) dest[i++] = item.second.get_value<double>();
        if (i != 3) throw std::length_error("json array must be of length 3");
        return true;
    }
    return false;
}

bool handleJsonOptionalList(boost::property_tree::ptree pt, const boost::property_tree::path path, std::map<boost::uuids::uuid,double>& dest) {
    boost::optional<boost::property_tree::ptree &> json = pt.get_child_optional(path);
    boost::uuids::string_generator uuid_gen;
    if (json) {
        for (boost::property_tree::ptree::value_type &elem : pt.get_child(path)) {
            boost::uuids::uuid uuid = uuid_gen(elem.first);
            dest[uuid] = elem.second.get_value<double>();
        }
        return true;
    }
    return false;
}

uint64_t getServerDate() {
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer [10];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer,10,"%Y%m%d",timeinfo);
    return (uint64_t)std::stoi(buffer);
}

Vehicle& CmdHandler::updateAVbyJson(char* recv) {
    std::stringstream ss;
    ss << recv;
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);
    boost::property_tree::ptree j_sensor = pt.get_child("sensor");

    if (j_sensor.get_value<std::string>() != "gps") {
        throw std::invalid_argument("json string does not contain gps sensor data");
    }

    // uuid
    boost::uuids::string_generator uuid_gen;
    boost::property_tree::ptree j_uuid = pt.get_child("uuid");
    boost::uuids::uuid uuid = uuid_gen(j_uuid.get_value<std::string>());

    // time
    boost::property_tree::ptree j_date = pt.get_child("date");
    boost::property_tree::ptree j_time = pt.get_child("time");
    uint64_t r_time = j_date.get_value<uint64_t>();
    if (r_time == 0UL) {
        r_time = getServerDate();
    }
    r_time *= 1000000000;
    r_time += j_time.get_value<uint64_t>();
    
    Vehicle& av = _stor.get(uuid);
    av.alpha.clear();
    double array[3];
    double value;

    av.aquireWrite();

    av.setUUID(uuid);

    if (handleJsonOptionalArray(pt, "position", array)) {
        av.setPosition(array[LAT],array[LON],array[ALT], r_time);
    }
    if (handleJsonOptionalArray(pt, "std_dev", array)) {
        av.setPosStdDev(array[LAT],array[LON],array[ALT], r_time);
    }
    if (handleJsonOptionalArray(pt, "orientation", array)) {
        av.setOrientation(array[YAW],array[PITCH],array[ROLL], r_time);
    } else if (handleJsonOptionalValue(pt, "course", &value)) {
        av.setOrientation(value,0.0,0.0, r_time);
    }
    try {
        if (handleJsonOptionalArray(pt, "velocity", array)) {
            av.setVelocity(array[X],array[Y],array[Z], r_time);
        }
    } catch (const std::length_error& e) {
        if (handleJsonOptionalValue(pt, "velocity", &value)) {
            av.setVelocity(value,0.0,0.0, r_time);
        } else {
            throw e;
        }
    }
    if (handleJsonOptionalArray(pt, "rotation", array)) {
        av.setRotation(array[YAW],array[PITCH],array[ROLL], r_time);
    }
    if (handleJsonOptionalArray(pt, "acceleration", array)) {
        av.setRotation(array[X],array[Y],array[Z], r_time);
    }
    if (handleJsonOptionalArray(pt, "ang_accel", array)) {
        av.setAngularAcceleration(array[YAW],array[PITCH],array[ROLL], r_time);
    }
    handleJsonOptionalList(pt, "alpha", av.alpha);

    boost::optional<double> j_perimeter = pt.get_optional<double>("perimeter");
    if (j_perimeter) av.setConfPerimeter(j_perimeter.get());
    boost::optional<time_t> j_max_age = pt.get_optional<time_t>("max_age");
    if (j_max_age) av.setConfMaxAge(j_max_age.get());
    av.releaseWrite();

    return av;
}

int CmdHandler::puts(char* recv, char* send) {
    try {
        Vehicle& av = updateAVbyJson(recv);
        std::memcpy(send, "OK\r\n", 5);
        return 5;
    } catch (const std::invalid_argument& e) {
        std::memcpy(send, "ERR: no gps data found in json string\r\n", 40);
        return 40;
    } catch (const boost::property_tree::ptree_bad_data& e) {
        std::memcpy(send, "ERR: bad data in json string\r\n", 31);
        return 42;
    } catch (const boost::property_tree::json_parser_error& e) {
        std::memcpy(send, "ERR: json format error, cannot parse\r\n", 39);
        return 39;
    } catch (const std::length_error& e) {
        std::memcpy(send, "ERR: json array must be of length 3\r\n", 38);
        return 38;
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
        std::memcpy(send, "ERR: no gps data found in json string\r\n", 40);
        return 40;
    } catch (const boost::property_tree::ptree_bad_data& e) {
        std::memcpy(send, "ERR: incomplete gps data in json string\r\n", 42);
        return 42;
    } catch (const boost::property_tree::json_parser_error& e) {
        std::memcpy(send, "ERR: json format error, cannot parse\r\n", 39);
        return 39;
    } catch (const std::length_error& e) {
        std::memcpy(send, "ERR: json array must be of length 3\r\n", 38);
        return 38;
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