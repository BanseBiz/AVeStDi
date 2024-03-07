#include "vehicle.hpp"
#include "storage.hpp"

#define VERSION "v240306\r\n"

class CmdHandler {
    public:
    static int vers(char*, char*);
    static int quit(char*, char*);
    static int show(char*, char*);
    static int puts(char*, char*);
    static int putl(char*, char*);

    private:
    static Vehicle& updateAVbyJson(char* recv);
    static Storage _stor;
};