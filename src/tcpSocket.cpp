
// https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux  
#include <stdio.h>  
#include <string.h>   //strlen  
#include <stdlib.h>  
#include <errno.h>  
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

#include "tcpSocket.h"

#define TRUE 1
#define FALSE 0
#define PORT 8888

TcpSocket::TcpSocket(
    unsigned int port,
    char* helo
) : 
     
int TcpSocket::init() {
    //initialise all client_socket[] to 0 so not checked  
    for (unsigned int i = 0; i < TCPSOCKET_MAXCLIENTS; ++i) {
        _client_socket[i] = 0;   
    }

    //create a master socket  
    if((_master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) { 
        perror("socket failed");
        return 1;
    }

    int opt = 1;
    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this  
    if(setsockopt(_master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {   
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    //type of socket created
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888  
    if (bind(_master_socket, (struct sockaddr *)&_address, sizeof(_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(_master_socket, TCPSOCKET_MAXPENDINGCONNECTIONS) < 0) {   
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    //accept the incoming connection  
    _addrlen = sizeof(_address);
    puts("Waiting for connections ...");
}

int TcpSocket::spin() {
    //set of socket descriptors  
    fd_set readfds;
    //clear the socket set  
    FD_ZERO(&readfds);   
    
    //add master socket to set  
    FD_SET(_master_socket, &readfds);   
    int max_sd = _master_socket;   
            
    //add child sockets to set  
    for (unsigned int i = 0; i < TCPSOCKET_MAXCLIENTS; i++) {   
        //socket descriptor  
        int sd = _client_socket[i];   
        
        //if valid socket descriptor then add to read list  
        if(sd > 0) FD_SET( sd , &readfds);   
        
        //highest file descriptor number, need it for the select function  
        if(sd > max_sd) max_sd = sd;   
    }

    //wait for an activity on one of the sockets , timeout is NULL ,  
    //so wait indefinitely      
    if (select(max_sd + 1 , &readfds , NULL , NULL , NULL) && (errno != EINTR)) {   
        printf("select error");
    }

    //If something happened on the master socket ,  
    //then its an incoming connection  
    if (FD_ISSET(_master_socket, &readfds)) {   
        int new_socket;
        if ((new_socket = accept(_master_socket, (struct sockaddr *)&_address, (socklen_t*)&_addrlen)) < 0) {   
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        //inform user of socket number - used in send and receive commands
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n",
            new_socket, inet_ntoa(_address.sin_addr) , ntohs(_address.sin_port)
        );   
        
        //send new connection greeting message
        if ( send(new_socket, _helo, strlen(_helo), 0) != strlen(_helo)) {
            perror("send");
        }

        puts("Welcome message sent successfully");
        //add new socket to array of sockets
        for (unsigned int i = 0; i < TCPSOCKET_MAXCLIENTS; ++i) {   
            //if position is empty  
            if (_client_socket[i] == 0) {
                _client_socket[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
            }
        }
    }
    
    //else its some IO operation on some other socket 
    for (unsigned int i = 0; i < TCPSOCKET_MAXCLIENTS; i++) {
        int sd = _client_socket[i];

        if (FD_ISSET(sd , &readfds)) {
            size_t msgsize;
            //Check if it was for closing , and also read the
            //incoming message
            if ((msgsize = read(sd , _buffer, TCPSOCKET_BUFFERSIZE)) == 0) {
                //Somebody disconnected , get his details and print
                getpeername(sd, (struct sockaddr*)&_address, (socklen_t*)&_addrlen);   
                printf("Host disconnected , ip %s , port %d \n" ,
                        inet_ntoa(_address.sin_addr) , ntohs(_address.sin_port));
                
                //Close the socket and mark as 0 in list for reuse  
                close(sd);
                _client_socket[i] = 0;
            } else {
                //set the string terminating NULL byte on the end
                //of the data read
                _buffer[msgsize] = '\0';
                send(sd, _buffer, strlen(_buffer), 0);
            }
        }
    }
    return 0;
}