#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <sys/socket.h>
#include <sys/time.h>

#include "../include/SocketVCANSender.h"
#include "../include/SocketCANReceiver.h"
#include "../include/VehicleDataRingBuffer.h"

int main() {
    std::cout << "Starting SocketCAN test..." << std::endl;

    VehicleDataRingBuffer queue(10);

    SocketVCANSender send_socket;
    send_socket.create_socket();
    std::thread sender(&SocketVCANSender::send_can_frames, &send_socket, 4);

    std::string interface_name = "vcan0";
    SocketCANReceiver recv_socket = SocketCANReceiver(interface_name, queue);
    std::thread receiver(&SocketCANReceiver::poll_data, &recv_socket);


    sender.detach();
    // At 4 Hz, 3 seconds is about 12 groups (the ring buffer holds 10)
    std::this_thread::sleep_for(std::chrono::seconds(3));
    int size = queue.get_size();
    std::cout << "size: " << size << std::endl; 
    receiver.detach();

    while (!queue.is_empty()) {
        VehicleData v = queue.pop_front();
        std::cout << "RPM: " << v.rpm << std::endl;
    }

    std::cout << "SocketCAN test complete." << std::endl;
    std::_Exit(0);  // kills the sender thread without joining it
}