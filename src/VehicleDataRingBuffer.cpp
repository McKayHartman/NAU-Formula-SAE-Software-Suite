#include "VehicleDataRingBuffer.h"

VehicleDataRingBuffer::VehicleDataRingBuffer(const int capacity) : capacity(capacity) {
    this->buffer.reserve(capacity);
};

bool VehicleDataRingBuffer::is_empty() const {
    return this->get_size(); // truthy/falsey
}

bool VehicleDataRingBuffer::is_full() const {
    if(this->get_size() == this->capacity) {
        return true;
    }
    return false;
}

VehicleData VehicleDataRingBuffer::pop_front() {
    if(this->is_empty()) {
        return VehicleData{};
    }
    VehicleData result = this->buffer[this->front];
    this->increment_front();
    return result;
}

Status VehicleDataRingBuffer::push_back(VehicleData v) {
    // Write-on-full increments back and front
    if(this->is_full()) {
        this->increment_front();
        this->increment_back();
        this->buffer[back] = v;
        return Status::SUCCESS;
    }
    this->increment_back();
    this->buffer[back] = v;
}

int VehicleDataRingBuffer::get_back() const {
    return this->back;
}

int VehicleDataRingBuffer::get_front() const {
    return this->front;
}

int VehicleDataRingBuffer::get_size() const {
    if(this->front == this->back) {
        return 0;
    }
    else if(this->front > this->back) {
        return this->back - this->front + 1 + this->capacity;
    }
    return this->back - this->front + 1;
}

void VehicleDataRingBuffer::increment_front() {
    this->front = (this->front + 1) % this->capacity;
}

void VehicleDataRingBuffer::increment_back() {
    this->back = (this->back + 1) % this->capacity;
}