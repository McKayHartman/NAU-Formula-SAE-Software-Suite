#pragma once
#include "VehicleDataRingBuffer.h"
#include <atomic>
#include <chrono>


namespace Constants {
    inline constexpr int hertz {20};
    inline constexpr int screen_res_w {400};
    inline constexpr int screen_res_h {400};
}

class DashboardUI {
    public:
        explicit DashboardUI(VehicleDataRingBuffer& buffer);

        void run();
        void stop();
    private:
        VehicleDataRingBuffer& buffer_;

        std::atomic<bool> running_{false};
};