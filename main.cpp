//
// Created by Michael Davis on 28/02/2018.
//

#include <zconf.h>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include "heartbeat.h"

// Server side C/C++ program to demonstrate Socket programming


#define PORT 8080


void receive_heartbeat_response(int sock, char* receiver, char* sender);

void send_malicious_heartbeat(int sock, char *payload, int payload_length, char *sender);

Heartbeat receive_heartbeat(int new_socket);

void receive_heartbeat_request_and_respond(int socket, char* current_host);

void send_and_receive_malicious_heartbeat_request(int sock, char *payload, int payload_length, char *host, char *victim);


int run_server_program();

int recv_all(int sockfd, void *buf, size_t len, int flags)
{
    size_t toread = len;
    char  *bufptr = (char*) buf;

    while (toread > 0)
    {
        ssize_t rsz = recv(sockfd, bufptr, toread, flags);
        if (rsz <= 0)
            return rsz;  /* Error or other end closed cnnection */

        toread -= rsz;  /* Read less next time */
        bufptr += rsz;  /* Next buffer position to read into */
    }

    return len;
}

using namespace std;


int heartbleed_option;


int main()
{

    const char *secret_username = "secret_username that no one should ever see :O";
    const char *secret_password = "secret password that is not to be shared!";
    const char *secret_key = "super secret key held in memory";
    const char* mess = "asdfjasdfkljasdfkljasdl;fkasjdf;skfaklsdfjasdl;kfjasdlfkjasdfl;kajsdflaksdjfalsdkjfdlkjadl;skfjasdl;fkdjasfladsjfldksaj;lsdkfjdals;kjfasl;kjsdf;dkfjasd;lfkjqweopirajsd;klfasldkfjs;alksfjsdl;kfjasdlkfjasdl;kfjasdlk;fjasd;lf";


    // Heartbleed options:
    // 1 : Heartbleed
    // 2 : Reverse Heartbleed
    bool valid_option = false;
    while(!valid_option) {
        cout << "---Heartbleed and Reverse Heartbleed Simulation---\n\n" << endl;
        cout << "Please select an option:" << endl;
        cout << "1: Heartbleed (collect data from server)" << endl;
        cout << "2: Reverse Heartbleed (collect data from client" << endl;

        cout << "Option: ";
        cin >> heartbleed_option;
        if (heartbleed_option == 1 || heartbleed_option == 2) {
            valid_option = true;
        } else {
            cout << "\n\nInvalid option\n\n" << endl;
        }
    }

    cout << "\n\n\n\n" << endl;


    int pid;

    pid = fork();

    if (pid < 0) { // error occurred
        fprintf(stderr, "Fork failed!\n");
        exit(-1);
    }else if (pid == 0) { // child process
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }

        int tr=1;

        // kill "Address already in use" error message
        if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
            printf("\nInvalid address/ Address not supported \n");
            return -1;
        }

        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }




        Heartbeat sneaky_heartbeat_request_send("cat", 1000);

        // Just info for user
        cout <<  ": Sending a malicious heartbeat request" << endl;
        cout << "Payload: " << sneaky_heartbeat_request_send.payload << endl;
        cout << "Payload length: " << sneaky_heartbeat_request_send.payload_length << endl;
        cout << "\n" << endl;

        // Send malicious Heartbeat Request
        stringstream string_stream;
        string_stream << sneaky_heartbeat_request_send;    //serialize
        send(sock, string_stream.str().c_str(), sizeof(string_stream.str()), 0);

        cout << "sent" << endl;

        Heartbeat heartbeat_response_receive = receive_heartbeat(sock);
        cout << ": Received Heartbeat Response" << endl;

        cout << "Response Payload (containing original payload, plus adjacent memory on " << "):" << endl;
        for (uint16_t i = 0; i < heartbeat_response_receive.payload_length; i++)
            cout << (heartbeat_response_receive.payload[i]);
        cout << endl;



       // return 0;

    } else { // parent process


        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        int tr=1;

        // kill "Address already in use" error message
        if (setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons( PORT );

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }



        while (1) {
            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }


            new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
            if ((new_socket < 0)) {
                perror("accept");
                exit(EXIT_FAILURE);
            } else {

                int server_pid = fork();
                if (server_pid < 0)
                    perror("ERROR on fork");
                if (server_pid == 0)  {
                    close(server_fd);

                    cout << "Connection accepted" << endl;
                    receive_heartbeat_request_and_respond(new_socket, "Server");
                    // char* c = "afdskldfjasldkfjasdfklsadjflksdjfas;ldkfjasl;dkfjasdlfkajsdflkadjsfl;askdfjsal;dfkjasdl;fkjasdlfk;ajsdf;lkasjdfal;skdfjasl;dfkqwepojvsdflkfdhgjkshjahfkldsfsdnbfmabsdfm,nabsdfnmasbdfesd;lfkjasd;lfkjasdf;lkajsdf;lkasjdf;lasdkjfa;lsdkfjas;ldkfjas;dlfkjasd;lfkjasdl;fkajsdfl;kasdjfakls;dfjasl;dkfjasd;lkfjasdfkjl";
/*
                    Heartbeat heartbeat_request_received = receive_heartbeat(new_socket);
                    cout << ": Heartbeat Request Received" << endl;
                    Heartbeat heartbeat_response_send = Heartbeat(&heartbeat_request_received);
                    stringstream ss;
                    ss << heartbeat_response_send;    //serialize
                    cout << "here" << endl;
                    send(new_socket, ss.str().c_str(), sizeof(ss.str()), 0);
                    cout <<": Heartbeat Response Sent\n" << endl;
*/

                    shutdown(new_socket, 0);
                    close(new_socket);
                    exit(0);
                }
            }

        }

    }
}




void send_and_receive_malicious_heartbeat_request(int sock, char *payload, int payload_length, char *host, char *victim) {
    send_malicious_heartbeat(sock, payload, payload_length, host);
    //cout << "" << endl;
    receive_heartbeat_response(sock, host, victim);
}

void receive_heartbeat_request_and_respond(int socket, char* current_host) {
    Heartbeat heartbeat_request_received = receive_heartbeat(socket);
    cout << current_host << ": Heartbeat Request Received" << endl;
    Heartbeat heartbeat_response_send = Heartbeat(&heartbeat_request_received);
    stringstream ss;
    ss << heartbeat_response_send;    //serialize
    send(socket, ss.str().c_str(), sizeof(ss.str()), 0);
    cout << current_host << ": Heartbeat Response Sent\n" << endl;
}


void send_malicious_heartbeat(int sock, char *payload, int payload_length, char *sender) {

    Heartbeat sneaky_heartbeat_request_send(payload, payload_length);

    // Just info for user
    cout << sender << ": Sending a malicious heartbeat request" << endl;
    cout << "Payload: " << sneaky_heartbeat_request_send.payload << endl;
    cout << "Payload length: " << sneaky_heartbeat_request_send.payload_length << endl;
    cout << "\n" << endl;

    // Send malicious Heartbeat Request
    stringstream string_stream;
    string_stream << sneaky_heartbeat_request_send;    //serialize
    write(sock, string_stream.str().c_str(), sizeof(string_stream.str()));
}


void receive_heartbeat_response(int sock, char* receiver, char* sender) {// Receive Heartbeat Response with server's data
    Heartbeat heartbeat_response_receive = receive_heartbeat(sock);
    cout << receiver << ": Received Heartbeat Response" << endl;

    cout << "Response Payload (containing original payload, plus adjacent memory on " << sender << "):" << endl;
    for (uint16_t i = 0; i < heartbeat_response_receive.payload_length; i++)
                cout << (heartbeat_response_receive.payload[i]);
    cout << endl;
}


Heartbeat receive_heartbeat(int new_socket) {
    Heartbeat heartbeat;
    stringstream string_stream;
    char buffer[sizeof(Heartbeat)];
    string temp;
    recv(new_socket, &buffer, sizeof(buffer), 0);  //receive
    temp.assign(buffer);
    string_stream << temp;
    string_stream >> heartbeat;   //unserialize
    cout << "asdf" << endl;
    return heartbeat;
}



