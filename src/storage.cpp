#include "storage.hpp"
#include <cstring>

Storage::Storage(/* args */) {

};

//int Storage::cleanup(double) {}


int Storage::put(boost::uuids::uuid uuid, double lat, double lon, double alt, time_t timestamp) {
    _nodes[uuid] = Vehicle(uuid, lat, lon, alt, timestamp);
    return 0;
}

Vehicle& Storage::get(boost::uuids::uuid uuid) {
    return _nodes[uuid];
}

bool Storage::contains(boost::uuids::uuid uuid) {
    std::map<boost::uuids::uuid,Vehicle>::iterator it = _nodes.find(uuid);
    return (it != _nodes.end());
}

int Storage::toCString(char* out, size_t max) const {
    if (_nodes.empty()) {
        std::memcpy(out,"[]",3);
        return 3;
    }
    out[0] = '[';
    int idx = 1;
    for (auto const& av : _nodes){
        idx += av.second.toCString(out + idx, max - idx);
        if (idx >= max-2) return -1;
        out[idx++] = ',';
    }
    out[idx-1] = ']';
    out[idx++] = '\0';
    return idx;
}

bool Storage::empty() const noexcept {
    return _nodes.empty();
}
