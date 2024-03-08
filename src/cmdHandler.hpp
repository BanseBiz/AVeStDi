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