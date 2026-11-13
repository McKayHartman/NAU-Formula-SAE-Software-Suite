#ifndef VEHICLEDATA_H
#define VEHICLEDATA_H

#include <cstdint>

struct VehicleData {
    uint16_t rpm;
    uint16_t tps;
    uint16_t map;
    uint16_t lambda;
    uint16_t lambdaA;
    uint16_t lambdaB;
    uint16_t ignition_angle;
    uint16_t ignition_cut;
    uint16_t fuel_pulse_width_primary;
    uint16_t fuel_duty_primary;
};

#endif