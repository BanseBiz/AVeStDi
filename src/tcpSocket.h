#pragma once

// https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  

#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>

#define TCPSOCKET_MAXCLIENTS 128
#define TCPSOCKET_MAXPENDINGCONNECTIONS 3
#define TCPSOCKET_BUFFERSIZE 16384

class TcpSocket
{
    public:
    TcpSocket(
        unsigned int port,
        char* helo
    );
    int init();
    int spin();
    
    private:
    const unsigned int _port;
    const char *_helo;

    int _client_socket[TCPSOCKET_MAXCLIENTS];
    int _master_socket;
    struct sockaddr_in _address;
    unsigned int _addrlen;

    char _buffer[TCPSOCKET_BUFFERSIZE];
};