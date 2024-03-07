#pragma once

// https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  

#include <sys/types.h>  
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>

#define TCPSOCKET_MAXCLIENTS 128
#define TCPSOCKET_MAXPENDINGCONNECTIONS 3
#define TCPSOCKET_RECVBUFFERSIZE 16384
#define TCPSOCKET_SENDBUFFERSIZE 16384

class TcpSocket
{
    public:
    typedef int (*action_t)(char*,char*);
    typedef struct {
        int socket = 0;
        boost::uuids::uuid uuid = { 
            0x00 ,0x00, 0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };
    } client_t;

    TcpSocket(
        unsigned int port,
        char* helo
    );
    int init();
    int spin();
    int addCmd(std::string, action_t);
    int registerClient(int, boost::uuids::uuid);
    ssize_t sendClient(boost::uuids::uuid, char*, size_t);
    
    private:
    const unsigned int _port;
    const char *_helo;

    const char _error_cmd_unknown[24] = "EXIT command unknown\r\n";

    client_t _clients[TCPSOCKET_MAXCLIENTS];
    int _master_socket;
    struct sockaddr_in _address;
    unsigned int _addrlen;

    char _recv_buffer[TCPSOCKET_RECVBUFFERSIZE];
    char _send_buffer[TCPSOCKET_SENDBUFFERSIZE];
    std::map<std::string,action_t> _cmd_map;
    std::map<boost::uuids::uuid,client_t*> _client_map;

    int handleCmd(size_t);
    int closeConn(int);
};