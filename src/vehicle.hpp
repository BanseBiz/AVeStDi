/* Copyright 2024 Marvin Banse

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
    int setPosStdDev(double, double, double, time_t);
    int setOrientation(double, double, double, time_t);
    int setVelocity(double, double, double, time_t);
    int setRotation(double, double, double, time_t);
    int setAcceleration(double, double, double, time_t);
    int setAngularAcceleration(double, double, double, time_t);

    int setConfPerimeter(double);
    int setConfMaxAge(time_t);

    std::array<double,3> getPosition() const;
    std::array<double,3> getOrientation() const;
    std::array<double,3> getVelocity() const;
    std::array<double,3> getRotation() const;
    std::array<double,3> getAcceleration() const;
    std::array<double,3> getAngularAcceleration() const;
    std::string toString() const;
    int toCString(char*,size_t) const;
    int toCString(char*,size_t,Vehicle&) const;
    boost::uuids::uuid getUUID() const;
    double getPerimeter() const;
    time_t getMaxAge() const;
    time_t getRecentUpdate() const;

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
    double _perimeter = 0.0;                         // distance in meters the av gets notified about ather avs
    time_t _max_age = 0L;                            // maximal age of AVs the av wants to be notified about

    time_t _recent_update[7] = {0L,0L,0L,0L,0L,0L,0L};
};