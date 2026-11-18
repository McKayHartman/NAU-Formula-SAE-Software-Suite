#include "../include/SocketVCANSender.h"

SocketVCANSender::SocketVCANSender(){};

SocketVCANSender::~SocketVCANSender(){
    close(this->socket_fd);
}

Status SocketVCANSender::create_socket() {
    struct sockaddr_can addr;
    struct ifreq ifr;
    const char* ifname = "vcan0"

    if ((this->socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
		perror("Error while opening socket");
		return Status::SOCK_ERROR;
	}

    strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(this->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		perror("Error in socket bind");
		return Status::SOCK_ERROR;
	}
}

static struct can_frame create_rpm_frame(uint16 rpm_val) {
    struct can_frame f;

    f.can_id = 0x520;
    f.can_dlc = 2;
    f.data[0] = rpm_val;
    f.data[1] = 0;

    return f;
}

static int calc_ms_delay(int Hz) {
    return 1000/Hz;
}


Status::SocketVCANSender::send_can_frames(int hertz) {
    int ms_delay = calc_ms_delay(hertz);
    uint16 rpm = 0;

    while(1) {
        struct can_frame frame = create_can_frame((rpm++ % 9000));
        nbytes = write(this->socket_fd, &frame, sizeof(struct can_frame));
        std::this_thread::sleep_for(std::chrono::milliseconds(ms_delay));
    }
}

