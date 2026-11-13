#ifndef VEHICLEDATARINGBUFFER_H
#define VEHICLEDATARINGBUFFER_H

#include <vector>

#include "VehicleData.h"

class VehicleDataRingBuffer {
private:
    std::vector<VehicleData> buffer;
    int front;
    int back;
    const int capacity;

public:
    VehicleDataRingBuffer(const int capacity);
    bool is_empty();
    bool is_full();

    const VehicleData pop_front();
    void push_back(VehicleData v);

    const int get_front();
    const int get_back();
    const int get_size();
};


#endif