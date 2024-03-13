/* Copyright 2024 Marvin Banse

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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

/* Calculates a catesian velocity vector in m/s from two distant positions, and the time of travel */
std::array<double,3> calcPOStoVEL(const std::array<double,3>&,const std::array<double,3>&,double);
std::array<double,3> calcPOStoVEL(const std::array<double,3>&,const std::array<double,3>&,double,std::array<double,3>&);

/* Calculates a orientation vector from two positions */
std::array<double,3> calcPOStoORI(std::array<double,3>&,std::array<double,3>&,std::array<double,3>&);

std::array<double,3> calcVELtoPOS(std::array<double,3>,double);
std::array<double,3> calcVELtoACC(std::array<double,3>,double);
std::array<double,3> calcACCtoVEL(std::array<double,3>,double);

std::array<double,3> calcORItoROT(std::array<double,3>,double);
std::array<double,3> calcROTtoORI(std::array<double,3>,double);
std::array<double,3> calcROTtoANG(std::array<double,3>,double);
std::array<double,3> calcANGtoROT(std::array<double,3>,double);

void rotateX(std::array<double,3>&,double);
void rotateY(std::array<double,3>&,double);
void rotateZ(std::array<double,3>&,double);
void rotate(std::array<double,3>&,std::array<double,3>);

std::array<double,3>& operator*=(std::array<double,3>&, const double);
std::array<double,3> operator*(std::array<double,3>&, const double&);
std::array<double,3>& operator*=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator*(std::array<double,3>&, const std::array<double,3>&);

std::array<double,3>& operator+=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator+(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3>& operator-=(std::array<double,3>&, const std::array<double,3>&);
std::array<double,3> operator-(std::array<double,3>&, const std::array<double,3>&);