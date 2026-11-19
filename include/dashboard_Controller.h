#pragma once
#include "VehicleDataRingBuffer.h"
#include <atomic>
#include <chrono>


namespace Constants {
    inline constexpr int hertz {20};
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