//
// Created by Michael Davis on 28/02/2018.
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "heartbeat.h"

Heartbeat::Heartbeat(char* payload, uint16_t payload_length) {
    this->message_type = heartbeat_request;
    this->payload_length = payload_length;
    this->payload = payload;

    for (char &i : this->padding)
        i = char(rand());
}

Heartbeat::Heartbeat(Heartbeat *request) throw() {
    if (request->message_type == heartbeat_request) {
        this->message_type = heartbeat_response;
        this->payload_length = request->payload_length;


       // this->payload = new char[payload_length];
        memcpy(&this->payload, &request->payload, request->payload_length);

        for (char &i : this->padding)
            i = rand();

    } else {
        throw std::invalid_argument("Invalid heartbeat type. Only requests can be given.");
    }
}

