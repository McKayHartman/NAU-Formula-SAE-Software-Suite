#include <iostream>

#include <thread>

#include "../include/SocketVCANSender.h"


int main() {
    SocketVCANSender s;
    auto val = s.create_socket();
    std::thread worker(&SocketVCANSender::send_can_frames, &s, 4);

    if (worker.joinable()) {
        worker.join();
    }
    return 0;
}