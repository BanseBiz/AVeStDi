#pragma once

#include <time.h>
#include <array>
#include "movement.hpp"

class Vehicle
{
    public:
    Vehicle();
    Vehicle(double, double, double, time_t);
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