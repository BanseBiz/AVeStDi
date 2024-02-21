#include <stdio.h> 
#include "tcpSocket.h"
#include <cstring>

#define PORT 8888
#define VERSION "v240221\r\n"

int vers(char* recv, char* send) {
    const char msg[] = VERSION;
    std::memcpy(send,msg,10);
    return 14;
}

int echo(char* recv, char* send) {
    size_t msg_len = strlen(recv);
    std::memcpy(send,recv,msg_len);
    return msg_len;
}

int main(int argc , char *argv[]) {
    char helo_msg[] = "HELO - AVeStDi daemon at " VERSION;
    TcpSocket tsck(PORT, helo_msg);
    tsck.init();
    tsck.addCmd("VERS", vers);
    tsck.addCmd("ECHO", echo);
    while(1) {
        tsck.spin();
    }
    return 0;
}