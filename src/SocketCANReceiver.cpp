// socketCAN receiver for Linux (will not run on Windows or mac)
#ifdef __linux__

#include "../include/SocketCANReceiver.h"

#include <cerrno>
#include <cstdio>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <linux/can.h>
#include <linux/can/raw.h>


namespace CanId
{
    constexpr canid_t FRAME_520 = 0x520;
    constexpr canid_t FRAME_521 = 0x521;
    constexpr canid_t FRAME_522 = 0x522;
}

enum : uint8_t
{
    GOT_520 = 1 << 0,
    GOT_521 = 1 << 1,
    GOT_522 = 1 << 2,
    GOT_ALL = GOT_520 | GOT_521 | GOT_522
};



SocketCANReceiver::SocketCANReceiver(const std::string& interface_name, VehicleDataRingBuffer& buff) : 
    interface_name(interface_name),
    buff(buff),
    fd(-1)
{
    fd = open_can_socket();

    if (fd < 0) {
        throw std::runtime_error("Failed to open SocketCAN interface: " + interface_name);
    }
}

SocketCANReceiver::~SocketCANReceiver() {
    close(this->fd);
}

int SocketCANReceiver::open_can_socket()
{
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) { perror("socket"); return -1; }

    sockaddr_can addr{};
    addr.can_family  = AF_CAN;
    addr.can_ifindex = if_nametoindex(this->interface_name.c_str());

    can_filter filters[] = {
        {CanId::FRAME_520, CAN_SFF_MASK},
        {CanId::FRAME_521, CAN_SFF_MASK},
        {CanId::FRAME_522, CAN_SFF_MASK},
    };
    timeval tv{0, 100000};

    if (addr.can_ifindex == 0 ||
        bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0 ||
        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, filters, sizeof(filters)) < 0 ||
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
    {
        perror("open_can_socket");
        close(s);
        return -1;
    }
    return s;
}

int SocketCANReceiver::get_fd() {
    return this->fd;
}

RecvStatus SocketCANReceiver::poll_data() {
    VehicleData current{};
    uint8_t byes_received = 0;
    std::cout << "Entering Loop" << std::endl;
    while(1) {
        std::cout << "-" << std::endl;
        can_frame frame{};
        ssize_t n = read(this->fd, &frame, sizeof(frame));
        std::cout << n << std::endl;
        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                // nothing for 100 ms: throw away any half-finished group
                byes_received = 0;
                current = VehicleData{};
                continue;
            }
            perror("read");
            return RecvStatus::ERROR;
        }

        if (n != sizeof(frame) || (frame.can_id & CAN_RTR_FLAG) || frame.can_dlc < 8)
            continue;

        const uint8_t* d = frame.data;
        switch (frame.can_id & CAN_SFF_MASK) {
            case CanId::FRAME_520:
                current.rpm    = u16(d, 0);
                current.tps    = u16(d, 2);
                current.map    = u16(d, 4);
                current.lambda = u16(d, 6);
                byes_received |= GOT_520;
                break;
            case CanId::FRAME_521:
                current.lambda_a       = u16(d, 0);
                current.lambda_b       = u16(d, 2);
                current.ignition_angle = u16(d, 4);
                current.ignition_cut   = u16(d, 6);
                byes_received |= GOT_521;
                break;
            case CanId::FRAME_522:
                current.fuel_pulse_width_primary = u16(d, 0);
                current.fuel_duty_primary        = u16(d, 2);
                byes_received |= GOT_522;
                break;
        }

        if (byes_received == GOT_ALL) {
            std::cout << "*" << std::endl;
            auto result = this->buff.push_back(current); // This line is commented out because the queue is not defined in this context
            if(static_cast<int>(result) > 0) {
                std::cout << "failed push_back()" << std::endl;
            }
            else {
                int size = buff.get_size();
                std::cout << "pushed success. queue size = " << size << std::endl; 
                VehicleData v = buff.pop_front();
                std::cout << "RPM = " << v.rpm << std::endl;
            }
            byes_received = 0;
            current = VehicleData{};
        }
    }
}



//// HELPER FUNCTIONS

static uint16_t u16(const uint8_t* d, int offset)
{
    return static_cast<uint16_t>(d[offset] | (d[offset + 1] << 8));
}

#endif