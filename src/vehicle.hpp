#pragma once

#include <time.h>

class Vehicle
{
    public:
    typedef enum posIdx_t {
        LAT = 0x1,
        LON = 0x2,
        ALT = 0x3
    };
    typedef enum dirIdx_t {
        X = 0x1,
        Y = 0x2,
        Z = 0x3
    };

    int tick(time_t);
    
    int setPosition(double, double, double, time_t);
    int setOrientation(double, double, double, time_t);
    int setVelocity(double, double, double, time_t);
    int setRotation(double, double, double, time_t);
    int setAcceleration(double, double, double, time_t);
    int setAngularAcceleration(double, double, double, time_t);
    
    private:
    double _position[3] = {0.0,0.0,0.0};             // latitude, longitude, altitude
    double _orientation[3] = {0.0,0.0,0.0};          // radians
    double _velocity[3] = {0.0,0.0,0.0};             // meters / second
    double _rotation[3] = {0.0,0.0,0.0};             // radians / second
    double _acceleration[3] = {0.0,0.0,0.0};         // meters per square second
    double _angular_acceleration[3] = {0.0,0.0,0.0}; // radians per square second
    double _distance = 0.0;

    time_t _recent_update;
};