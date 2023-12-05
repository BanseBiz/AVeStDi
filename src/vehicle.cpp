#include "vehicle.hpp"
#include <GeographicLib/Geodesic.hpp>

int Vehicle::setPosition(double lat, double lon, double alt, time_t timestamp) {
    if (timestamp < _recent_update[0]) return 1;
    _recent_update[0] = timestamp;
    _position[LAT] = lat;
    _position[LON] = lon;
    _position[ALT] = alt;
    return 0;
}

int Vehicle::setOrientation(double yaw, double pitch, double roll, time_t timestamp) {
    if (timestamp < _recent_update[1]) return 1;
    _recent_update[1] = timestamp;
    _orientation[YAW] = yaw;
    _orientation[PITCH] = pitch;
    _orientation[ROLL] = roll;
    return 0;
}

int Vehicle::setVelocity(double x, double y, double z, time_t timestamp) {
    if (timestamp < _recent_update[2]) return 1;
    _recent_update[2] = timestamp;
    _velocity[X] = x;
    _velocity[Y] = y;
    _velocity[Z] = z;
    return 0;
}

int Vehicle::setRotation(double yaw, double pitch, double roll, time_t timestamp) {
    if (timestamp < _recent_update[3]) return 1;
    _recent_update[3] = timestamp;
    _rotation[YAW] = yaw;
    _rotation[PITCH] = roll;
    _rotation[ROLL] = roll;
    return 0;
}

int Vehicle::setAcceleration(double x, double y, double z, time_t timestamp) {
    if (timestamp < _recent_update[4]) return 1;
    _recent_update[4] = timestamp;
    _acceleration[X] = x;
    _acceleration[Y] = y;
    _acceleration[Z] = z;
    return 0;
}

int Vehicle::setAngularAcceleration(double yaw, double pitch, double roll, time_t timestamp) {
    if (timestamp < _recent_update[5]) return 1;
    _recent_update[5] = timestamp;
    _angular_acceleration[YAW] = yaw;
    _angular_acceleration[PITCH] = pitch;
    _angular_acceleration[ROLL] = roll;
    return 0;
}

std::array<double,3> Vehicle::getPosition() {
    std::array<double,3> pos = {_position[LAT], _position[LON], _position[ALT]};
    return pos;
}

std::array<double,3> Vehicle::getOrientation() {
    std::array<double,3> orient = {_orientation[YAW], _orientation[PITCH], _orientation[ROLL]};
    return orient;
}

std::array<double,3> Vehicle::getVelocity() {
    std::array<double,3> vel = {_velocity[X], _velocity[Y], _velocity[Z]};
    return vel;
}

std::array<double,3> Vehicle::getRotation() {
    std::array<double,3> rot = {_rotation[YAW], _rotation[PITCH], _rotation[ROLL]};
    return rot;
}

std::array<double,3> Vehicle::getAcceleration() {
    std::array<double,3> acc = {_acceleration[X], _acceleration[Y], _acceleration[Z]};
    return acc;
}

std::array<double,3> Vehicle::getAngularAcceleration() {
    std::array<double,3> rot_acc = {_angular_acceleration[YAW], _angular_acceleration[PITCH], _angular_acceleration[ROLL]};
    return rot_acc;
}