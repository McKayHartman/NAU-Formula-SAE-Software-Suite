// socketCAN receiver implementation for Linux
// will not run on Windows or mac
#ifdef __linux__

#include "SocketCANReceiver.h"

#include <cstring>
#include <cstdio>
#include <chrono>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cerrno>
#include <linux/can.h>
#include <linux/can/raw.h>

//can ids from the ECU, each frame can hold diff vehicle data
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

// combines two bytes into one 16 bit value
static uint16_t u16(const uint8_t* d, int offset) 
{
    return static_cast<uint16_t>(d[offset] | (d[offset + 1] << 8));
}

// opens the can socket to receive data
int open_can_socket(const std::string& interface_name) 
{
    
    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0) { perror("socket"); return -1; }

    // gets can interface
    ifreq ifr{};
    std::strncpy(ifr.ifr_name, interface_name.c_str(), IFNAMSIZ - 1);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) 
    {
        perror("ioctl SIOCGIFINDEX");
        close(s);
        return -1;
    }

    // connects the socket to the can interface
    sockaddr_can addr{};
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) 
    {
        perror("bind");
        close(s);
        return -1;
    }

    // sets the socket to only receive the frames we care about
    can_filter filters[] = {
        {CanId::FRAME_520, CAN_SFF_MASK},
        {CanId::FRAME_521, CAN_SFF_MASK},
        {CanId::FRAME_522, CAN_SFF_MASK},
    };
    
    if (setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, filters, sizeof(filters)) < 0) 
    {
        perror("setsockopt CAN_RAW_FILTER");
        close(s);
        return -1;
    }
    
    // waits up to 100 ms for new data
    timeval tv{0, 100000};  

    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) 
    {
        perror("setsockopt SO_RCVTIMEO");
        close(s);
        return -1;
    }

    return s;
}

// reads incoming can frames and stores vehicle data
// keeps running until `running` is set to false; closes socket_fd on exit
void poll_data(int socket_fd,
               VehicleDataRingBuffer& queue,
               std::atomic<bool>& running)
{
    VehicleData current{};
    uint8_t received = 0;

    using clock = std::chrono::steady_clock;

    constexpr auto frame_timeout = std::chrono::milliseconds(100);
    auto group_start = clock::now();

    while (running) 
    {
        can_frame frame{};
        ssize_t n = read(socket_fd, &frame, sizeof(frame));

        auto now = clock::now();
        
        // clears old data if we haven't received all frames in time
        if (received != 0 && now - group_start >= frame_timeout) 
        {
            received = 0;
            current = VehicleData{};
        }
        // checks if there was an error reading the socket 
        if (n < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                continue;
            }
            perror("read");
            break;
        }

        // skips frames that are not complete or not needed
        if (n != sizeof(frame)) continue;
        if (frame.can_id & (CAN_ERR_FLAG | CAN_RTR_FLAG)) continue;
        if (frame.can_dlc < 8) continue;

        if (received == 0) 
        {
            group_start = clock::now();
        }

        // checks can id and saves correct vehicle data
        const uint8_t* d = frame.data;
        switch (frame.can_id & CAN_SFF_MASK) 
        {
            case CanId::FRAME_520:
                current.rpm    = u16(d, 0);
                current.tps    = u16(d, 2);
                current.map    = u16(d, 4);
                current.lambda = u16(d, 6);
                received |= GOT_520;
                break;
            case CanId::FRAME_521:
                current.lambda_a      = u16(d, 0);
                current.lambda_b      = u16(d, 2);
                current.ignition_angle = u16(d, 4);
                current.ignition_cut   = u16(d, 6);
                received |= GOT_521;
                break;
            case CanId::FRAME_522:
                current.fuel_pulse_width_primary = u16(d, 0);
                current.fuel_duty_primary       = u16(d, 2);
                received |= GOT_522;
                break;
        }
        // adds vehile data to the queue once all froms arrive
        if (received == GOT_ALL) 
        {
            queue.push_back(current);
            received = 0;
        }
    }

    close(socket_fd);
}

#endif 
