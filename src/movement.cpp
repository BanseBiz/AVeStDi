#include "movement.hpp"

std::array<double,3> calcPOStoVEL(std::array<double,3>,double duration) {

}

std::array<double,3> calcVELtoPOS(std::array<double,3> velocity ,double duration) {
    velocity *= duration;
}

std::array<double,3> calcVELtoACC(std::array<double,3>,double duration) {

}

std::array<double,3> calcACCtoVEL(std::array<double,3>,double duration) {

}

std::array<double,3> calcORItoROT(std::array<double,3>,double duration) {

}

std::array<double,3> calcROTtoORI(std::array<double,3>,double duration) {

}

std::array<double,3> calcROTtoANG(std::array<double,3>,double duration) {

}

std::array<double,3> calcANGtoROT(std::array<double,3>,double duration) {
    
}

std::array<double,3>& operator*=(std::array<double,3>& vec, const double& mul) {
    vec[X] *= mul;
    vec[Y] *= mul;
    vec[Z] *= mul;
    return vec;
}

std::array<double,3> operator*(std::array<double,3>& vec, const double& mul) {
    return {vec[X] * mul, vec[Y] * mul, vec[Z] * mul};
}