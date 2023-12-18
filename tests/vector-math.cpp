#include "../src/movement.hpp"
#include <cstdio>

int main() {
    int result = 0;
    // *= num
    {
        std::array<double,3> vec = {2,-4,10};
        double num = 1.5;
        vec *= num;
        if (vec[X] != 3 || vec[Y] != -6 || vec[Z] != 15) {
            result++;
            printf("vec *= num gives invalid results: %f %f %f\n", vec[X], vec[Y], vec[Z]);
        }
    }

    // * num
    {
        std::array<double,3> vec2 = {1,-2,3};
        double num = 2;
        std::array<double,3> vec1 = vec2 * num;
        if (vec1[X] != 2 || vec1[Y] != -4 || vec1[Z] != 6) {
            result++;
            printf("vec * num gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    // *= vec
    {
        std::array<double,3> vec1 = {1,2,1};
        std::array<double,3> vec2 = {2,4,1};
        vec1 *= vec2;
        if (vec1[X] != -2 || vec1[Y] != 1 || vec1[Z] != 0) {
            result++;
            printf("vec *= vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    // * vec
    {
        std::array<double,3> vec2 = {1,2,1};
        std::array<double,3> vec3 = {2,4,1};
        std::array<double,3> vec1 = vec2 * vec3;
        if (vec1[X] != -2 || vec1[Y] != 1 || vec1[Z] != 0) {
            result++;
            printf("vec * vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }
    
    // += vec
    {
        std::array<double,3> vec2 = {1,2,1};
        std::array<double,3> vec1 = {2,4,1};
        vec1 += vec2;
        if (vec1[X] != 3 || vec1[Y] != 6 || vec1[Z] != 2) {
            result++;
            printf("vec += vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    // + vec
    {
        std::array<double,3> vec2 = {1,2,1};
        std::array<double,3> vec3 = {2,4,1};
        std::array<double,3> vec1 = vec2 + vec3;
        if (vec1[X] != 3 || vec1[Y] != 6 || vec1[Z] != 2) {
            result++;
            printf("vec + vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    // -= vec
    {
        std::array<double,3> vec1 = {1,2,1};
        std::array<double,3> vec2 = {2,4,1};
        vec1 -= vec2;
        if (vec1[X] != -1 || vec1[Y] != -2 || vec1[Z] != 0) {
            result++;
            printf("vec -= vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    // - vec
    {
        std::array<double,3> vec2 = {1,2,1};
        std::array<double,3> vec3 = {2,4,1};
        std::array<double,3> vec1 = vec2 - vec3;
        if (vec1[X] != -1 || vec1[Y] != -2 || vec1[Z] != 0) {
            result++;
            printf("vec - vec gives invalid results: %f %f %f\n", vec1[X], vec1[Y], vec1[Z]);
        }
    }

    if (result > 0) return 1;
    return 0;
}