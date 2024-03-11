#pragma once

#include "vehicle.hpp"
#include <map>
#include <boost/uuid/uuid.hpp>

class Storage
{
private:
    std::map<boost::uuids::uuid,Vehicle> _nodes;
    time_t _max_age = 0;

public:
    Storage(/* args */);

//    int cleanup(double);  //maximal accepted distance
    int put(boost::uuids::uuid, double, double, double, time_t);
    Vehicle& get(boost::uuids::uuid);
    bool contains(boost::uuids::uuid);
    int toCString(char*,size_t) const;
    int toCString(char*,size_t,Vehicle&) const;
    bool empty() const noexcept;
    void setMaxAge(time_t);
};