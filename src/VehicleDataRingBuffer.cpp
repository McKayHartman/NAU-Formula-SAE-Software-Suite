#include "VehicleDataRingBuffer.h"

VehicleDataRingBuffer::VehicleDataRingBuffer(const int capacity) : capacity(capacity) {
    this->buffer.reserve(capacity);
};

bool VehicleDataRingBuffer::is_empty() {
    return this->get_size(); // truthy/falsey
}

bool VehicleDataRingBuffer::is_full() {
    if(this->get_size() == this->capacity) {
        return true;
    }
    return false;
}

void VehicleDataRingBuffer::push_back(VehicleData v) {
    if
}
