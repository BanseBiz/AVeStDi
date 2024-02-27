#include "storage.hpp"

Storage::Storage(/* args */) {

};

//int Storage::cleanup(double) {}


int Storage::put(boost::uuids::uuid uuid, double lat, double lon, double alt, time_t timestamp) {
    _nodes[uuid] = Vehicle(lat, lon, alt, timestamp);
    return 0;
}

Vehicle& Storage::get(boost::uuids::uuid uuid) {
    return _nodes[uuid];
}

bool Storage::contains(boost::uuids::uuid uuid) {
    std::map<boost::uuids::uuid,Vehicle>::iterator it = _nodes.find(uuid);
    return (it != _nodes.end());
}