#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include "queue.h"

using namespace std;

int main() {
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        cout << "Could not create socket" << endl;
    }
    cout << "Socket created" << endl;

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }

    cout << "Connected" << endl;

    //keep communicating with server
    while(1) {
        cout << "Enter message : " << endl;
        scanf("%s" , message);

        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0) {
            cout << "Send failed" << endl;
            return 1;
        }

        int read_size;
        if( (read_size = recv(sock , server_reply , 2000 , 0)) < 0) {
            cout << "recv failed" << endl;
            break;
        }
        server_reply[read_size] = 0;

        cout << "Server reply :" << server_reply << endl;
    }

    close(sock);
    return 0;
}

