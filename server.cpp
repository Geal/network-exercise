#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include "queue.h"

using namespace std;

int main() {
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        cout << "Could not create socket" << endl;
    }
    cout << "Socket created" << endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    if( ::bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    cout << "bind done" << endl;

    listen(socket_desc , 3);

    cout << "Waiting for incoming connections..." << endl;
    c = sizeof(struct sockaddr_in);

    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    cout << "Connection accepted" << endl;

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ) {
        cout << "client sent:" << client_message << endl;
        write(client_sock , client_message , strlen(client_message));
    }

    if(read_size == 0) {
        cout << "Client disconnected" << endl;
        fflush(stdout);
    } else if(read_size == -1) {
        perror("recv failed");
    }

    return 0;
}
