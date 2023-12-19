#include "../src/movement.hpp"
#include <cstdio>
#include <array>

int main() {
    time_t duration = 2;
    std::array<double,3> ori = {0.0, 0.0, 0.0};
    std::array<double,3> pos_1 = {53.120656, 8.205386, 2};
    std::array<double,3> pos_2 = {53.120582, 8.205281, 3};
    std::array<double,3> vel = calcPOStoVEL(pos_1,pos_2,duration,ori);
    printf("velocity results: %f %f %f\n", vel[LAT], vel[LON], vel[ALT]);
    return 1;
}