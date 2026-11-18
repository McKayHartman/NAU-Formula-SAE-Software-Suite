#ifndef SOCKETCANRECEIVER_H
#define SOCKETCANRECEIVER_H

#include <atomic>
#include <string>

#include "VehicleDataRingBuffer.h"

// SocketCAN is linux-only these are only implemented with linux 

// opens a socketcan connection using the given interface
int open_can_socket(const std::string& interface_name);

// reads can frames and pushes complete vehicle data samples into the queue
void poll_data(int socket_fd,
               VehicleDataRingBuffer& queue,
               std::atomic<bool>& running);

#endif
