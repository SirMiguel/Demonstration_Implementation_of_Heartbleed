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

// Server side C/C++ program to demonstrate Socket programming

#define PORT 8080


unsigned char * create_sneaky_heartbeat_request(unsigned int payload_length){
    unsigned char *buffer, *p;
    buffer = new unsigned char[1 + 2 + payload_length];
    p = buffer;
    *p++ =  0;
    *p++ = payload_length;
    *p++ = payload_length >> 8;
    return buffer;
}

//*p++ = 'c';
//*p++ = 'a';
//*p++ = 't';
//or (int i = 0; i < payload_length; i++){
//   *p++ = rand();//(unsigned char) rand();
//}

void read_heartbeat_response(int socket) {
    unsigned char *type_buf = new unsigned char();
    read(socket, type_buf, 1);
    unsigned short type = (unsigned short) *type_buf;

    if (type == 1) {
        cout << "\nReading Heartbeat Response" << endl;
        cout << "Type received: " << type << endl;

        unsigned char *payload_length_buf = new unsigned char[2];

        read(socket, payload_length_buf, 2);

        unsigned int payload_length = payload_length_buf[0] << 8 | payload_length_buf[1];//(unsigned int) *payload_length_buf;
        cout << "Received Payload length:" << payload_length << endl;

        unsigned char *payload_buffer = new unsigned char[payload_length];
        read(socket, payload_buffer, payload_length);

        cout << "Payload from victim:";
        for (int i = 0; i < payload_length; i++) {
            if (i % 50 == 0) {
                cout << endl;
            }
            cout << (int) payload_buffer[i] << " "; //(unsigned int)
        }
        cout << endl;
    }
}

void read_and_respond_to_heartbeat_request(int socket) {
    unsigned char* type_buf = new unsigned char();
    read(socket, type_buf, 1);
    unsigned short type = (unsigned short) *type_buf;
   // cout << "Type received: " << type << endl;

    if (type == 0){
        //cout << "\nReceived Heartbeat Request" << endl;
        unsigned char *payload_length_buf = new unsigned char[2];

        read(socket, payload_length_buf, 2);

        unsigned int payload_length = payload_length_buf[0] << 8 | payload_length_buf[1];//(unsigned int) *payload_length_buf;
        //cout << "Payload Length:" << payload_length << endl;
        unsigned char* payload_buffer = new unsigned char[payload_length];
        read(socket, payload_buffer, payload_length);
       // unsigned int* payload = (unsigned int*) payload_buffer;

        /*cout << "Payload:";
        for (int i = 0; i < payload_length; i++)
            cout << (unsigned int)payload_buffer[i];
        cout << endl;*/

        //unsigned char* received_heartbeat_request = new unsigned char[1 + 2 + payload_length];
        int total_buffer_length = 1 + 2 + payload_length;
        unsigned char* buffer = new unsigned char[total_buffer_length];
        unsigned char* b = buffer;
        *b++ = 1;
        *b++ = payload_length;
        *b++ = payload_length >> 8;
        memcpy(b, payload_buffer, payload_length);


        /*cout << "Sending Heatbeat Response buffer" << endl;
        for (int i = 0; i < sizeof(buffer); i++)
            cout << (int)buffer[i];
        cout << endl;
        cout << endl;
        */
        write(socket, buffer, sizeof(buffer));
    }
}

void send_sneaky_heartbeat_request(int socket, int payload_length) {
    // MAX PAYLOAD LENGTH: 255
    unsigned char * heartbeat_request_to_send = create_sneaky_heartbeat_request(payload_length);
    cout << "\nSending Heatbeat Request: ";
    for (int i = 0; i < sizeof(heartbeat_request_to_send); i++) {
        cout << static_cast<int>(heartbeat_request_to_send[i]);
            cout << " ";
    }
    cout << endl;

    ssize_t sent = write(socket, heartbeat_request_to_send, sizeof(heartbeat_request_to_send));
    //cout << "Sent: " << sent << endl;
}


void run_heartbleed_as_baddie(){
}


using namespace std;


int heartbleed_option;


int main() {


    // Heartbleed options:
    // 1 : Heartbleed
    // 2 : Reverse Heartbleed
    bool valid_option = false;
    while (!valid_option) {
        cout << "---Heartbleed and Reverse Heartbleed Simulation---\n" << endl;
        cout << "Please select an option:" << endl;
        cout << "1: Heartbleed (collect data from server)" << endl;
        cout << "2: Reverse Heartbleed (collect data from client" << endl;
        cout << "\nNote: to quit while running either Heartbleed or Reverse Hearbleed, please press 'q'\n" << endl;
        cout << "Option: ";
        cin >> heartbleed_option;
        if (heartbleed_option == 1 || heartbleed_option == 2) {
            valid_option = true;
        } else {
            cout << "\n\nInvalid option\n\n" << endl;
        }
    }


    int bytes_to_steal = 0;
    cout << "Please enter the number of bytes to steal from victim (max is 65535):";
    cin >> bytes_to_steal;

    cout << "\n\n" << endl;

    int pid;

    pid = fork();

    if (pid < 0) { // error occurred
        fprintf(stderr, "Fork failed!\n");
        exit(-1);
    } else if (pid == 0) { // child process
        // CLIENT SET UP
        struct sockaddr_in address;
        int sock = 0, valread;
        struct sockaddr_in serv_addr;
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(-1);
        }

        int tr = 1;

        // kill "Address already in use" error message
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        memset(&serv_addr, '0', sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);


        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            exit(-1);
        }

        if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            printf("\nConnection Failed \n");
            exit(-1);
        }


        // HEARTBEAT CODE
        if (heartbleed_option == 1) {
            while (cin.get() != 'q') {
                cout << "\n\n\nInstance of Heartbeat" << endl;

                send_sneaky_heartbeat_request(sock, bytes_to_steal);
                read_heartbeat_response(sock);
                cout << "To continue reading bytes please press any key and press enter, otherwise enter 'q' to quit" << endl;
            }
            close(sock);

        } else if (heartbleed_option == 2) {
            while (1) {
                read_and_respond_to_heartbeat_request(sock);
            }
        }

        return 0;

    } else { // parent process
        // SERVER SET UP
        int server_fd, new_socket, valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        int tr = 1;

        // kill "Address already in use" error message
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        // HEARTBEAT CODE
        if (heartbleed_option == 1) {
                new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
                if (new_socket < 0) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                } else {
                    while (1) {

                        /*  int server_pid = fork();
                          if (server_pid < 0) {
                              fprintf(stderr, "Fork failed!\n");
                              exit(-1);
                          }
                          if (server_pid == 0) {*/
                        close(server_fd);
                        read_and_respond_to_heartbeat_request(new_socket);
                }
            }
        } else if (heartbleed_option == 2) {
            new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen);
            if (new_socket < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            } else {
                while (cin.get() != 'q') {
                    cout << "\n\n\nInstance of Reverse Heartbeat" << endl;
                    send_sneaky_heartbeat_request(new_socket, bytes_to_steal);
                    read_heartbeat_response(new_socket);
                    cout << "To continue reading bytes please press any key and press enter, otherwise enter 'q' to quit" << endl;
                }
                exit(0);
            }
        }
    }
    return 0;
}

