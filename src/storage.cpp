#include "storage.hpp"
#include <cstring>

Storage::Storage(/* args */) {

};

//int Storage::cleanup(double) {}


int Storage::put(boost::uuids::uuid uuid, double lat, double lon, double alt, time_t timestamp) {
    if (!contains(uuid)) _nodes.insert({uuid, Vehicle(uuid)});
    _nodes[uuid].setPosition(lat, lon, alt, timestamp);
    return 0;
}

Vehicle& Storage::get(boost::uuids::uuid uuid) {
    if (!contains(uuid)) _nodes.insert({uuid, Vehicle(uuid)});
    return _nodes[uuid];
}

bool Storage::contains(boost::uuids::uuid uuid) {
    std::map<boost::uuids::uuid,Vehicle>::iterator it = _nodes.find(uuid);
    return (it != _nodes.end());
}

int Storage::toCString(char* out, size_t max) {
    if (_nodes.empty()) {
        std::memcpy(out,"[]",3);
        return 3;
    }
    out[0] = '[';
    int idx = 1;
    int inkr;
    for (auto &av : _nodes){
        inkr = av.second.toCString(out + idx, max - idx);
        if (inkr <= 0) continue;
        idx += inkr;
        if (idx >= max-2) return -1;
        out[idx++] = ',';
    }
    out[idx-1] = ']';
    out[idx++] = '\0';
    return idx;
}

int Storage::toCString(char* out, size_t max, Vehicle& reference) {
    if (_nodes.size() < 2) {
        std::memcpy(out,"[]",3);
        return 3;
    }
    out[0] = '[';
    int idx = 1;
    int inkr;
    for (auto &av : _nodes){
        if (av.first == reference.getUUID()) continue;
        inkr = av.second.toCString(out + idx, max - idx, reference);
        if (inkr <= 0) continue;
        idx += inkr;
        if (idx >= max-2) return -1;
        out[idx++] = ',';
    }
    if (idx == 1) {
        out[idx++] = ']';
    } else {
        out[idx-1] = ']';
    }
    out[idx++] = '\0';
    return idx;
}

bool Storage::empty() const noexcept {
    return _nodes.empty();
}
