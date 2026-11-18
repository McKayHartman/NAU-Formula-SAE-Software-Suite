#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstring>

struct class Status {
    SUCCESS,
    ERROR,
    WARNING
}

class [[nodiscard]] VcanSender {
private:
    int socket_fd;

public:
    Status open_can();
    Status close_can();

    ssize_t send_can_frame(const can_frame& frame);
}
