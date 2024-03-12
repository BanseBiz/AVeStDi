#include "vehicle.hpp"
#include <GeographicLib/Constants.hpp>
#include <boost/uuid/uuid_io.hpp>

Vehicle::Vehicle()
 : _geod(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f()) {

};

Vehicle::Vehicle(boost::uuids::uuid uuid)
 : _uuid(uuid), _geod(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f()) {

};

Vehicle::Vehicle(boost::uuids::uuid uuid, double lat, double lon, double alt, time_t timestamp)
 : _uuid(uuid), _geod(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f()) {
    setPosition(lat, lon, alt, timestamp);
};

int Vehicle::setPosition(double lat, double lon, double alt, time_t timestamp) {
    if (timestamp < _recent_update[0]) return 1;
    _recent_update[0] = timestamp;
    _position[LAT] = lat;
    _position[LON] = lon;
    _position[ALT] = alt;
    return 0;
}

int Vehicle::setPosStdDev(double lat, double lon, double alt, time_t timestamp) {
    if (timestamp < _recent_update[6]) return 1;
    _recent_update[6] = timestamp;
    _pos_std_dev[LAT] = lat;
    _pos_std_dev[LON] = lon;
    _pos_std_dev[ALT] = alt;
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

std::array<double,3> Vehicle::getPosition() const {
    std::array<double,3> pos = {_position[LAT], _position[LON], _position[ALT]};
    return pos;
}

std::array<double,3> Vehicle::getOrientation() const {
    std::array<double,3> orient = {_orientation[YAW], _orientation[PITCH], _orientation[ROLL]};
    return orient;
}

std::array<double,3> Vehicle::getVelocity() const {
    std::array<double,3> vel = {_velocity[X], _velocity[Y], _velocity[Z]};
    return vel;
}

std::array<double,3> Vehicle::getRotation() const {
    std::array<double,3> rot = {_rotation[YAW], _rotation[PITCH], _rotation[ROLL]};
    return rot;
}

std::array<double,3> Vehicle::getAcceleration() const {
    std::array<double,3> acc = {_acceleration[X], _acceleration[Y], _acceleration[Z]};
    return acc;
}

std::array<double,3> Vehicle::getAngularAcceleration() const {
    std::array<double,3> rot_acc = {_angular_acceleration[YAW], _angular_acceleration[PITCH], _angular_acceleration[ROLL]};
    return rot_acc;
}

int Vehicle::tick(time_t now) {
    time_t elapsed_time[6] = {0};
    unsigned int mode = POS; // POS is 0
    for (unsigned int i = 5; i >= 0; --i) {
        elapsed_time[i] = now - _recent_update[i];
        if (elapsed_time[i] < elapsed_time[mode]) {
            mode = i; //recent datum
        }
    }
    for (unsigned int i = 0; i < 6; ++i) {
        if (elapsed_time[i] != 0) {
            switch (i) {
                case POS: {
                    //
                    break;
                }
                case ORI: {
                    break;
                }
                case VEL: {
                    break;
                }
                case ROT: {
                    break;
                }
                case ACC: {
                    break;
                }
                case ANG: {
                    break;
                }
            }
        }
    }

    return 0;
}

boost::uuids::uuid Vehicle::getUUID() const {
    return _uuid;
}

double Vehicle::getSearchPerimeter() const {
    return _search_perimeter;
}

time_t Vehicle::getRecentUpdate() const {
    return _recent_update[0];
}

int Vehicle::toCString(char* out, size_t max) const {
    const std::string s_uuid = boost::uuids::to_string(_uuid);
    return snprintf(out, max,
        "{\"type\":\"ground\","
        "\"uuid\":\"%s\","
        "\"timestamp\":\"%lu\","
        "\"position\":[%.8f,%.8f,%.3f],"
        "\"std_dev\":[%.2f,%.2f,%.2f],"
        "\"orientation\":[%.4f,%.4f,%.4f],"
        "\"velocity\":[%.4f,%.4f,%.4f]}",
        s_uuid.c_str(), _recent_update,
        _position[LAT], _position[LON], _position[ALT],
        _pos_std_dev[LAT], _pos_std_dev[LON], _pos_std_dev[ALT],
        _orientation[YAW], _orientation[PITCH], _orientation[ROLL],
        _velocity[X], _velocity[Y], _velocity[Z]
    );
}

int Vehicle::toCString(char* out, size_t max, Vehicle& reference, time_t max_age) const {
    if ((max_age > 0) && ((getRecentUpdate() + max_age) < reference.getRecentUpdate())) {
        return 0;
    }
    
    const std::string s_uuid = boost::uuids::to_string(_uuid);
    double s12 = 0.0; //Distance
    double azi1 = 0.0; //Fwd. Azimuth
    double azi2 = 0.0; //Backw. Azimuth
    double direction = 0.0;
    std::array<double,3UL> ref_pos = reference.getPosition();
    _geod.Inverse(
        _position[LAT],
        _position[LON],
        ref_pos[LAT],
        ref_pos[LON],
        s12, azi1, azi2
    );

    if (reference.getSearchPerimeter() > 0.0 && reference.getSearchPerimeter() > s12) {
        return 0;
    }

    return snprintf(out, max,
        "{\"type\":\"ground\","
        "\"uuid\":\"%s\","
        "\"timestamp\":\"%lu\","
        "\"position\":[%.8f,%.8f,%.3f],"
        "\"std_dev\":[%.2f,%.2f,%.2f],"
        "\"orientation\":[%.4f,%.4f,%.4f],"
        "\"velocity\":[%.4f,%.4f,%.4f],"
        "\"distance\":%.4f,"
        "\"direction\":%.4f}",
        s_uuid.c_str(), _recent_update,
        _position[LAT], _position[LON], _position[ALT],
        _pos_std_dev[LAT], _pos_std_dev[LON], _pos_std_dev[ALT],
        _orientation[YAW], _orientation[PITCH], _orientation[ROLL],
        _velocity[X], _velocity[Y], _velocity[Z],
        s12, azi1
    );
}