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

#include "vehicle.hpp"
#include <map>
#include <boost/uuid/uuid.hpp>

class Storage
{
private:
    std::map<boost::uuids::uuid,Vehicle> _nodes;

public:
    Storage(/* args */);

//    int cleanup(double);  //maximal accepted distance
    int put(boost::uuids::uuid, double, double, double, time_t);
    Vehicle& get(boost::uuids::uuid);
    bool contains(boost::uuids::uuid);
    int toCString(char*,size_t);
    int toCString(char*,size_t,Vehicle&);
    bool empty() const noexcept;
    void setMaxAge(time_t);
};