#pragma once

#include <vector>
#include "vehicle.hpp"

class Storage
{
private:
    std::vector<Vehicle> _nodes;

public:
    Storage(/* args */);
    ~Storage();

    int cleanup(double);  //maximal accepted distance
    int put(Vehicle);
    Vehicle& get(__uint128_t);
};