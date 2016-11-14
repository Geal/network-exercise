#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <iostream>
#include <thread>
#include "queue.h"

using namespace std;

void connection_thread(int id, int sock);

int main() {
    int socket_desc, c;
    struct sockaddr_in server;

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

    int id = 0;
    while(true) {
      struct sockaddr_in client;
      int client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
      if (client_sock < 0) {
          perror("accept failed");
          return 1;
      }
      cout << "[" << id << "] Connection accepted" << endl;
      std::thread t(connection_thread, id, client_sock);
      t.detach();
      id++;
    }
    return 0;
}

void connection_thread(int id, int sock) {
    int read_size;
    char client_message[2000];

    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 ) {
        client_message[read_size] = 0;
        cout << "[" << id << "]:" << client_message << endl;
        write(sock , client_message , read_size);
    }

    if(read_size == 0) {
        cout << "[" << id << "] disconnected" << endl;
        fflush(stdout);
    } else if(read_size == -1) {
        cout << "[" << id << "]recv failed" << endl;
        fflush(stdout);
    }
}
