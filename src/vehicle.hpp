#pragma once

#include <time.h>
#include <array>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <GeographicLib/Geodesic.hpp>
#include "movement.hpp"

class Vehicle
{
    public:
    Vehicle();
    Vehicle(boost::uuids::uuid);
    Vehicle(boost::uuids::uuid, double, double, double, time_t);
    int tick(time_t);
    
    int setPosition(double, double, double, time_t);
    int setPosStdDev(double, double, double);
    int setOrientation(double, double, double, time_t);
    int setVelocity(double, double, double, time_t);
    int setRotation(double, double, double, time_t);
    int setAcceleration(double, double, double, time_t);
    int setAngularAcceleration(double, double, double, time_t);

    std::array<double,3> getPosition();
    std::array<double,3> getOrientation();
    std::array<double,3> getVelocity();
    std::array<double,3> getRotation();
    std::array<double,3> getAcceleration();
    std::array<double,3> getAngularAcceleration();
    std::string toString();
    int toCString(char*,size_t) const;
    int toCString(char*,size_t,Vehicle&) const;
    boost::uuids::uuid getUUID() const;
    
    private:
    boost::uuids::uuid _uuid;
    GeographicLib::Geodesic _geod;
    double _position[3] = {0.0,0.0,0.0};             // latitude, longitude, altitude
    double _pos_std_dev[3] = {0.0,0.0,0.0};          // positional standard deviation in metres
    double _orientation[3] = {0.0,0.0,0.0};          // radians
    double _velocity[3] = {0.0,0.0,0.0};             // meters / second
    double _rotation[3] = {0.0,0.0,0.0};             // radians / second
    double _acceleration[3] = {0.0,0.0,0.0};         // meters per square second
    double _angular_acceleration[3] = {0.0,0.0,0.0}; // radians per square second
    double _area_of_effect = 0.0;                    // distance in meters the av gets
                                                     //   notified about ather avs

    time_t _recent_update[6];
};