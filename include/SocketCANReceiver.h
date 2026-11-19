#ifndef SOCKETCANRECEIVER_H
#define SOCKETCANRECEIVER_H

#include <atomic>
#include <string>
#include <iostream>
#include "VehicleDataRingBuffer.h"




enum class [[nodiscard]] RecvStatus {
    SUCCESS,
    ERROR
};




class SocketCANReceiver {
public:
    SocketCANReceiver(const std::string& interface_name, VehicleDataRingBuffer& buff);
    ~SocketCANReceiver();

    int get_fd();
    int open_can_socket();

    RecvStatus poll_data();
    
private:
    int fd = -1;
    std::string interface_name;
    VehicleDataRingBuffer buff;
    
    
};



static uint16_t u16(const uint8_t* d, int offset);

#endif
