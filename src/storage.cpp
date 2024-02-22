#include "storage.hpp"

Storage::Storage(/* args */) {

};

//int Storage::cleanup(double) {}


int Storage::put(__uint128_t uuid, double lat, double lon, double alt, time_t timestamp) {
    _nodes[uuid] = Vehicle(lat, lon, alt, timestamp);
    return 0;
}

Vehicle& Storage::get(__uint128_t uuid) {
    return _nodes[uuid];
}

bool Storage::contains(__uint128_t uuid) {
    std::map<__uint128_t,Vehicle>::iterator it = _nodes.find(uuid);
    return (it != _nodes.end());
}