//
// Created by Michael Davis on 28/02/2018.
//

#ifndef DEMONSTRATION_IMPLEMENTATION_HEARTBEAT_H
#define DEMONSTRATION_IMPLEMENTATION_HEARTBEAT_H

#include <cstdint>

using namespace std;

class Heartbeat {
public:
    enum HeartbeatMessageType {
        heartbeat_request,
        heartbeat_response
    };

    HeartbeatMessageType message_type;
    uint16_t payload_length; // Max value of 65,535
    char* payload;
    char padding[16];
    Heartbeat (char *payload, uint16_t payload_length);
    Heartbeat (Heartbeat *request) throw();

    Heartbeat(){}

    friend ostream& operator<< (ostream& out, Heartbeat& heartbeat) {
        int message_int = (heartbeat.message_type == HeartbeatMessageType::heartbeat_request) ? 0 : 1;
        out << message_int << " "
            << heartbeat.payload_length << " "
            << heartbeat.payload << " "
            << heartbeat.padding;   //The space (" ") is necessari for separete elements
        return out;
    }


    friend istream& operator>> (istream& in, Heartbeat& heartbeat) {
        int message_int;
        in >> message_int;
        heartbeat.message_type = (message_int == 0) ? HeartbeatMessageType::heartbeat_request : HeartbeatMessageType::heartbeat_response;
        in >> heartbeat.payload_length;
        in >> heartbeat.payload;
        in >> heartbeat.padding;
        return in;
    }
};

#endif //DEMONSTRATION_IMPLEMENTATION_HEARTBEAT_H
