/*
//
// Created by Michael Davis on 28/02/2018.
//

#include <cstring>
#include <cstdlib>
#include "heartbeat_struct.h"
*/
/*

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

Heartbeat::Heartbeat* send_request(char *payload, uint16_t payload_length){
    auto* request = new Heartbeat;
    request->message_type = request->HeartbeatMessageType::heartbeat_request;
    request->payload_length = payload_length;
    request->payload = payload;

    for (char &i : request->padding)
        i = char(rand());

    return request;
}

Heartbeat::Heartbeat* send_response(Heartbeat *request){
    auto* response = new Heartbeat;
    response->message_type = response->HeartbeatMessageType::heartbeat_response;
    response->payload_length = request->payload_length;

    memcpy(&response->payload, &request->payload, request->payload_length);

    for (char &i : response->padding)
        i = rand();
    return response;
}
 */