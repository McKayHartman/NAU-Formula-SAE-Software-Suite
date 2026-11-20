#include "../include/SocketVCANSender.h"

SocketVCANSender::SocketVCANSender(){};

SocketVCANSender::~SocketVCANSender(){
    close(this->socket_fd);
}

SendStatus SocketVCANSender::create_socket() {
    struct sockaddr_can addr;
    struct ifreq ifr;
    const char* ifname = "vcan0";

    if ((this->socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
		perror("Error while opening socket");
		return SendStatus::SOCK_ERROR;
	}

    strcpy(ifr.ifr_name, ifname);
	ioctl(this->socket_fd, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(this->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("Error in socket bind");
		return SendStatus::SOCK_ERROR;
	}
}

static struct can_frame create_frame(uint16_t id, uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
    struct can_frame f{};
    f.can_id = id;
    f.can_dlc = 8;
    f.data[0] = a & 0xFF;  f.data[1] = a >> 8;
    f.data[2] = b & 0xFF;  f.data[3] = b >> 8;
    f.data[4] = c & 0xFF;  f.data[5] = c >> 8;
    f.data[6] = d & 0xFF;  f.data[7] = d >> 8;
    return f;
}

static int calc_ms_delay(int Hz) {
    return 1000/Hz;
}


SendStatus SocketVCANSender::send_can_frames(int hertz) {
    int ms_delay = calc_ms_delay(hertz);
    uint16_t rpm = 0;

    while(1) {
        struct can_frame f520 = create_frame(0x520, rpm++ % 9000, 100, 50, 1000);
        struct can_frame f521 = create_frame(0x521, 1000, 1000, 15, 0);
        struct can_frame f522 = create_frame(0x522, 2500, 40, 0, 0);
        write(this->socket_fd, &f520, sizeof(struct can_frame));
        write(this->socket_fd, &f521, sizeof(struct can_frame));
        write(this->socket_fd, &f522, sizeof(struct can_frame));
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_delay));
    }

    return SendStatus::SUCCESS;
}

