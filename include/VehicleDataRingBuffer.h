#ifndef VEHICLEDATARINGBUFFER_H
#define VEHICLEDATARINGBUFFER_H

#include <vector>

#include "VehicleData.h"

enum class [[nodiscard]] Status {
    SUCCESS,
    FAILURE,
    BAD_PARAMS,
};

class VehicleDataRingBuffer {
private:
    std::vector<VehicleData> buffer;
    int front = 0;
    int back = 0;
    const int capacity;

    void increment_front();
    void increment_back();

public:
    VehicleDataRingBuffer(const int capacity);
    bool is_empty() const;
    bool is_full() const;

    VehicleData pop_front();
    Status push_back(VehicleData v);

    int get_front() const;
    int get_back() const;
    int get_size() const;


};


#endif