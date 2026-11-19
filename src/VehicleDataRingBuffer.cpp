#include "../include/VehicleDataRingBuffer.h"

VehicleDataRingBuffer::VehicleDataRingBuffer(const int capacity) 
    : buffer(capacity > 0 ? capacity : 0), capacity(capacity) {}


bool VehicleDataRingBuffer::is_empty() const {
    return this->get_size() == 0; // truthy/falsey
}

bool VehicleDataRingBuffer::is_full() const {
    return this->get_size() == this->capacity - 1;
}

VehicleData VehicleDataRingBuffer::pop_front() {
    std::lock_guard<std::mutex> lock(mutex);
    if(this->is_empty()) {
        return VehicleData{};
    }
    this->increment_front();
    VehicleData result = this->buffer[this->front];
    return result;
}

BufStatus VehicleDataRingBuffer::push_back(VehicleData v) {
    std::lock_guard<std::mutex> lock(mutex);
    // Write-on-full increments back and front
    if(this->is_full()) {
        this->increment_front();
        this->increment_back();
        this->buffer[back] = v;
        return BufStatus::SUCCESS;
    }
    this->increment_back();
    this->buffer[back] = v;
    return BufStatus::SUCCESS;
}

int VehicleDataRingBuffer::get_back() const {
    return this->back;
}

int VehicleDataRingBuffer::get_front() const {
    return this->front;
}

int VehicleDataRingBuffer::get_size() const {
    return (this->back - this->front + this->capacity) % this->capacity;
}

void VehicleDataRingBuffer::increment_front() {
    this->front = (this->front + 1) % this->capacity;
}

void VehicleDataRingBuffer::increment_back() {
    this->back = (this->back + 1) % this->capacity;
}