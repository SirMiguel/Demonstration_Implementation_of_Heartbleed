/*
#include <iostream>
#include <random>

using namespace std;

struct Heartbeat {

    enum HeartbeatMessageType {
        heartbeat_request,
        heartbeat_response
    };


    HeartbeatMessageType message_type;
    uint16_t payload_length; // Max value of 65,535
    char* payload;
    char padding[16];
};

Heartbeat* send_request(char *payload, uint16_t payload_length){
    auto* request = new Heartbeat;
    request->message_type = request->HeartbeatMessageType::heartbeat_request;
    request->payload_length = payload_length;
    request->payload = payload;

    for (char &i : request->padding)
        i = char(rand());

    return request;
}

Heartbeat* send_response(Heartbeat *request){
    auto* response = new Heartbeat;
    response->message_type = response->HeartbeatMessageType::heartbeat_response;
    response->payload_length = request->payload_length;

    memcpy(&response->payload, &request->payload, request->payload_length);

    for (char &i : response->padding)
        i = rand();
    return response;
}

int main() {

    const char* secret_username = "secret_username";
    const char* secret_password = "secret password";
    const char* secret_key = "super secret key held in memory";



    char* message = "A totally legitimate payload message";
    uint16_t message_length = 400;

    Heartbeat* request = send_request(message, message_length);
    Heartbeat* response = send_response(request);

    int chars_per_line = 50;

    for (uint16_t i = 0; i < response->payload_length; i++) {
            cout << (response->payload[i]);

        if (i % chars_per_line == 0 && i > 0){
            cout << endl;
        }
    }

    delete request;
    delete response;
}
*/