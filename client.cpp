#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

#include "queue.h"
#include "option.h"

using namespace std;

void network_thread(int socket, Queue<string>& in);

int main() {
    int sock;
    struct sockaddr_in server;
    char message[2000];
    char username[30];

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        cout << "Could not create socket" << endl;
    }
    cout << "Socket created" << endl;

    fcntl(sock, F_SETFL, O_NONBLOCK);

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINPROGRESS) {
            this_thread::sleep_for(chrono::milliseconds(10));
        } else {
            perror("connect failed");
            //cout << "connect failed: " << errno << endl;

            //fflush(stdout);
            exit(1);
        }
    }

    cout << "Connected" << endl;

    Queue<string> q;
    std::thread network(network_thread, sock, ref(q));
    this_thread::sleep_for(chrono::milliseconds(10));

    cout << "Enter your username:" << endl;
    scanf("%s", username);

    //keep communicating with server
    while(1) {
        cout << "<" << username << ">\t";
        scanf("%s" , message);
        ostringstream os;
        os << "<" << username << ">\t" << message;
        q.enqueue(os.str());
    }

    close(sock);
    return 0;
}

void network_thread(int socket, Queue<string>& in) {
    char server_reply[2001];
    int read_size;
    cout << "network thread created" << endl;
    while(true) {
        auto message = in.try_dequeue(10);

        if(message->is_some()) {
            while(true) {
                read_size = write(socket, message->get().c_str(), message->get().length());
                if(read_size == 0) {
                    cout << "server disconnected" << endl;
                    fflush(stdout);
                    exit(1);
                } else if(read_size == -1) {
                    if (errno == EWOULDBLOCK || errno == EAGAIN) {
                        this_thread::sleep_for(chrono::milliseconds(10));
                        continue;
                    } else {
                        cout << "send failed: " << errno << endl;
                        fflush(stdout);
                        exit(1);
                    }
                } else {
                    //message sent, not taking real size into account
                    break;
                }
            }
        }

        read_size = read(socket, server_reply, 2000);
        if(read_size == 0) {
            cout << "server disconnected" << endl;
            fflush(stdout);
            exit(1);
        } else if(read_size == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                this_thread::sleep_for(chrono::milliseconds(50));
                continue;
            } else {
                cout << "recv failed: " << errno << endl;
                fflush(stdout);
                exit(1);
            }
        } else {
            server_reply[read_size] = 0;
            cout << server_reply;
        }
    }
}

