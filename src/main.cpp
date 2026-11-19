#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <sys/socket.h>
#include <sys/time.h>

#include "../include/SocketVCANSender.h"
#include "../include/SocketCANReceiver.h"
#include "../include/VehicleDataRingBuffer.h"
#include "../include/DashboardController.h"

int main() {
    std::cout << "Starting SocketCAN test..." << std::endl;

    VehicleDataRingBuffer queue(10);

    SocketVCANSender send_socket;
    send_socket.create_socket();

    std::string interface_name = "vcan0";
    SocketCANReceiver recv_socket(interface_name, queue);

    std::thread sender(
        &SocketVCANSender::send_can_frames,
        &send_socket,
        50
    );

    std::thread receiver(
        &SocketCANReceiver::poll_data,
        &recv_socket
    );

    ui(queue);

    std::cout << "SocketCAN test complete." << std::endl;
}