#include "movement.hpp"

std::array<double,3> calcPOStoVEL(std::array<double,3> position, double duration) {
    return position;
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

std::array<double,3> calcORItoROT(std::array<double,3> orientation ,double duration) {
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