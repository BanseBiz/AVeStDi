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

    int setPosition(double, double, double, time_t);
    int setOrientation(double, double, double, time_t);
    int setVelocity(double, double, double, time_t);
    int setRotation(double, double, double, time_t);
    int setAcceleration(double, double, double, time_t);
    int setAngularAcceleration(double, double, double, time_t);
    
    private:
    double _position[3];                // latitude, longitude, altitude
    double _orientation[3];             // radians
    double _velocity[3];                // meters / second
    double _rotation[3];                // radians / second
    double _acceleration[3];            // meters per square second
    double _angular_acceleration[3];    // radians per square second
    time_t _recent_update;
};