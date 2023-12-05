#pragma once

#include <time.h>
#include <array>

class Vehicle
{
    public:
    enum posIdx_t {
        LAT = 0x0,
        LON = 0x1,
        ALT = 0x2
    };
    enum orientIdx_t {
        YAW = 0x0,
        PITCH = 0x1,
        ROLL = 0x2
    };
    enum dirIdx_t {
        X = 0x0,
        Y = 0x1,
        Z = 0x2
    };

    int tick(time_t);
    
    int setPosition(double, double, double, time_t);
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
    
    private:
    double _position[3] = {0.0,0.0,0.0};             // latitude, longitude, altitude
    double _orientation[3] = {0.0,0.0,0.0};          // radians
    double _velocity[3] = {0.0,0.0,0.0};             // meters / second
    double _rotation[3] = {0.0,0.0,0.0};             // radians / second
    double _acceleration[3] = {0.0,0.0,0.0};         // meters per square second
    double _angular_acceleration[3] = {0.0,0.0,0.0}; // radians per square second
    double _distance = 0.0;

    time_t _recent_update[6];
};