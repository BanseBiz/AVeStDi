#include "movement.hpp"
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Constants.hpp>
#include <cmath>

std::array<double,3> calcPOStoVEL(const std::array<double,3>& pos_1, const std::array<double,3>& pos_2, double duration) {
    if (duration < 0.0001) return {0.0,0.0,0.0};
    GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
    double s12, azi1, azi2;
    geod.Inverse(pos_1[LAT], pos_1[LON], pos_2[LAT], pos_2[LON], s12);
    std::array<double,3> velocity = {
        s12 / duration, 
        0.0,
        (pos_2[ALT] - pos_1[ALT]) / duration
    };
    return velocity;
}

std::array<double,3> calcPOStoVEL(const std::array<double,3>& pos_1, const std::array<double,3>& pos_2,
    double duration, std::array<double,3>& orientation) {
    if (duration < 0.0001) return {0.0,0.0,0.0};
    GeographicLib::Geodesic geod(GeographicLib::Constants::WGS84_a(), GeographicLib::Constants::WGS84_f());
    double s12, azi1, azi2;
    geod.Inverse(pos_1[LAT], pos_1[LON], pos_2[LAT], pos_2[LON], s12, azi1, azi2);

    double dazm = atan2(sin(azi1),cos(azi2));  //WRONG
    // delta azm atan2( sin(winkel), cos(winkel) )
    // convert to radians
    // 

    std::array<double,3> velocity = {
        s12 / duration, 
        azi2,
        (pos_2[ALT] - pos_1[ALT]) / duration
    };
    return velocity;
}

std::array<double,3> calcVELtoPOS(std::array<double,3> velocity, double duration) {
    velocity *= duration;
    return velocity;
}

std::array<double,3> calcVELtoACC(std::array<double,3> velocity, double duration) {
    return velocity;
}

std::array<double,3> calcACCtoVEL(std::array<double,3> acceleration, double duration) {
    return acceleration;
}

std::array<double,3> calcORItoROT(std::array<double,3> orientation, double duration) {
    return orientation;
}

std::array<double,3> calcROTtoORI(std::array<double,3> rotation, double duration) {
    return rotation;
}

std::array<double,3> calcROTtoANG(std::array<double,3> rotation, double duration) {
    return rotation;
}

std::array<double,3> calcANGtoROT(std::array<double,3> rotation, double duration) {
    return rotation;
}

void rotateX(std::array<double,3>& vec, double rad) {
    double rot[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, cos(rad), -sin(rad)},
        {0.0, sin(rad), cos(rad)}
    };
    double a = rot[X][X]*vec[X]+rot[X][Y]*vec[Y]+rot[X][Z]*vec[Z];
    double b = rot[Y][X]*vec[X]+rot[Y][Y]*vec[Y]+rot[Y][Z]*vec[Z];
    double c = rot[Z][X]*vec[X]+rot[Z][Y]*vec[Y]+rot[Z][Z]*vec[Z];
}

void rotateY(std::array<double,3>& vec, double rad) {
    double rot[3][3] = {
        {cos(rad), 0.0, sin(rad)},
        {0.0, 1.0, 0.0},
        {-sin(rad), 0.0, cos(rad)}
    };
}

void rotateZ(std::array<double,3>& vec, double rad) {
    double rot[3][3] = {
        {cos(rad), -sin(rad), 0.0},
        {sin(rad), cos(rad), 0.0},
        {0.0, 0.0, 1.0}
    };
}

void rotate(std::array<double,3>& vec, std::array<double,3> rad) {
    double rot[3][3] = {
        {cos(rad[Z])*cos(rad[Y]), (cos(rad[Z])*sin(rad[Y])*sin(rad[X])) - (sin(rad[Z])*cos(rad[X])), (cos(rad[Z])*sin(rad[Y])*cos(rad[X])) + (sin(rad[Z])*sin(rad[X]))},
        {sin(rad[Z])*cos(rad[Y]), (sin(rad[Z])*sin(rad[Y])*sin(rad[X])) + (cos(rad[Z])*cos(rad[X])), (sin(rad[Z])*sin(rad[Y])*cos(rad[X])) - (cos(rad[Z])*sin(rad[X]))},
        {-sin(rad[Y]), cos(rad[Y])*sin(rad[X]), cos(rad[Y])*cos(rad[X])}
    };
}

std::array<double,3>& operator*=(std::array<double,3>& vec, const double mul) {
    vec[X] *= mul;
    vec[Y] *= mul;
    vec[Z] *= mul;
    return vec;
}

std::array<double,3> operator*(std::array<double,3>& vec, const double& mul) {
    return {vec[X] * mul, vec[Y] * mul, vec[Z] * mul};
}

std::array<double,3>& operator*=(std::array<double,3>& a, const std::array<double,3>& b) {
    double x = (a[Y]*b[Z]) - (a[Z]*b[Y]);
    double y = (a[Z]*b[X]) - (a[X]*b[Z]);
    double z = (a[X]*b[Y]) - (a[Y]*b[X]);
    a[X] = x;
    a[Y] = y;
    a[Z] = z;
    return a;
}

std::array<double,3> operator*(std::array<double,3>& a, const std::array<double,3>& b) {
    return {
        (a[Y]*b[Z]) - (a[Z]*b[Y]),
        (a[Z]*b[X]) - (a[X]*b[Z]),
        (a[X]*b[Y]) - (a[Y]*b[X])
    };
}

std::array<double,3>& operator+=(std::array<double,3>& a, const std::array<double,3>& b) {
    a[X] += b[X];
    a[Y] += b[Y];
    a[Z] += b[Z];
    return a;
}

std::array<double,3> operator+(std::array<double,3>& a, const std::array<double,3>& b) {
    return {
        a[X] + b[X],
        a[Y] + b[Y],
        a[Z] + b[Z]
    };
}

std::array<double,3>& operator-=(std::array<double,3>& a, const std::array<double,3>& b) {
    a[X] -= b[X];
    a[Y] -= b[Y];
    a[Z] -= b[Z];
    return a;
}

std::array<double,3> operator-(std::array<double,3>& a, const std::array<double,3>& b) {
    return {
        a[X] - b[X],
        a[Y] - b[Y],
        a[Z] - b[Z]
    };
}