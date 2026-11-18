#include "dashboard_UI.h"
#include "../include/VehicleDataRingBuffer.h"
#include <chrono>
#include <string>
#include <iostream>

int main(int argc, char **argv)
{
    int sim_RPM = 0;
    VehicleDataRingBuffer buffer(151);
    for(int i = 0; i<10000; i+= 100) {
        VehicleData sim;
        sim.rpm = i;
        Status s = buffer.push_back(sim);


    }
    auto dashboard = dashboard_Basic::create();
    
    //make slint timer to update every 50 milliseconds (20 Hz)
    slint::Timer update_timer(std::chrono::milliseconds(50), [&]() {
        if (!(buffer.is_empty())) {
            //pop rpm from stack
            sim_RPM = buffer.pop_front().rpm;
            dashboard->set_RPM(sim_RPM);
        }
        
    });

    // Show the window and spin the event loop until the window is closed.
    dashboard->run();
    
    return 0;
}