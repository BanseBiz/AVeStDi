#pragma once

#include "vehicle.hpp"
#include <map>

class Storage
{
private:
    std::map<__uint128_t,Vehicle> _nodes;

public:
    Storage(/* args */);

//    int cleanup(double);  //maximal accepted distance
    int put(__uint128_t, double, double, double, time_t);
    Vehicle& get(__uint128_t);
    bool contains(__uint128_t);
};