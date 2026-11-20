#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <thread>
#include <chrono>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/can.h>
#include <linux/can/raw.h>

enum class [[nodiscard]] SendStatus {
    SUCCESS,
    ERROR,
    SOCK_ERROR,
    SEND_ERROR
};

class SocketVCANSender {
public:
    SocketVCANSender();
    ~SocketVCANSender();

    SendStatus create_socket();
    SendStatus send_can_frames(int hertz);
private:
    int socket_fd = -1;
};