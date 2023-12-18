#pragma once
#include <array>
#include <time.h>

enum posIdx_t {
    LAT = 0x0,
    LON = 0x1,
    ALT = 0x2
};
enum orientIdx_t {
    YAW = 0x0,
    PITCH = 0x1,
    ROLL = 0x2
};
enum dirIdx_t {
    X = 0x0,
    Y = 0x1,
    Z = 0x2
};
enum typeIdx_t {
    POS = 0x0,
    ORI = 0x1,
    VEL = 0x2,
    ROT = 0x3,
    ACC = 0x4,
    ANG = 0x5
};

/* */
std::array<double,3> calcPOStoVEL(std::array<double,3>,double);
std::array<double,3> calcVELtoPOS(std::array<double,3>,double);
std::array<double,3> calcVELtoACC(std::array<double,3>,double);
std::array<double,3> calcACCtoVEL(std::array<double,3>,double);

std::array<double,3> calcORItoROT(std::array<double,3>,double);
std::array<double,3> calcROTtoORI(std::array<double,3>,double);
std::array<double,3> calcROTtoANG(std::array<double,3>,double);
std::array<double,3> calcANGtoROT(std::array<double,3>,double);

std::array<double,3>& operator*=(std::array<double,3>&, const double);
std::array<double,3> operator*(std::array<double,3>&, const double&);
std::array<double,3>& operator*=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator*(std::array<double,3>&, const std::array<double,3>&);

std::array<double,3>& operator+=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator+(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3>& operator-=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator-(std::array<double,3>&, const std::array<double,3>&);