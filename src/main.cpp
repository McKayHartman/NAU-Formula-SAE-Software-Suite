#include <iostream>

#include <thread>

#include "../include/SocketVCANSender/h"


int main() {
    SocketVCANSender s;
    s.create_socket();
    std::thread worker(s.send_can_frames, 4);

    if (worker.joinable()) {
        worker.join();
    }
    return 0;
}