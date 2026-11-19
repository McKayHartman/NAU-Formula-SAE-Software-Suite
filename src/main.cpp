#include <iostream>

#include <thread>

#include "../include/SocketVCANSender.h"
#include "../include/SocketCANReceiver.h"


int main() {
    SocketVCANSender s;
    auto val = s.create_socket();
    std::thread sender(&SocketVCANSender::send_can_frames, &s, 4);

    if (sender.joinable()) {
        sender.join();
    }

    
    return 0;
}