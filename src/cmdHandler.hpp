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

#include "vehicle.hpp"
#include "storage.hpp"
#include <string>

class CmdHandler {
    public:
    static int vers(char*, char*);
    static int quit(char*, char*);
    static int show(char*, char*);
    static int puts(char*, char*);
    static int putl(char*, char*);

    static std::string _version;

    private:
    static Vehicle& updateAVbyJson(char* recv);
    static Storage _stor;
};