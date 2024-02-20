#include <stdio.h> 
#include "tcpSocket.h"

#define PORT 8888
     
int main(int argc , char *argv[]) {
    char* helo_msg = "Ohhi, this is wip, just echo";
    TcpSocket tsck(PORT, helo_msg);
    tsck.init();

    while(1) {
        tsck.spin();
    }
}