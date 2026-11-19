#include "dashboard_UI.h"
#include "../include/dashboard_Controller.h"
#include <slint.h>

DashboardUI::DashboardUI(VehicleDataRingBuffer &buffer) : buffer_(buffer) {};

void DashboardUI::run() {
    auto dashboard = dashboard_Basic::create();
    slint::Timer update_timer(std::chrono::seconds(1/Constants::hertz), [&]() {
        if (!(buffer_.is_empty())) {
            //pop rpm from stack

            dashboard->set_RPM(buffer_.pop_front().rpm);
        }
        
    });
    running_ = true;
    // Show the window and spin the event loop until the window is closed.
    dashboard->run();
    running_ = false;
}

void DashboardUI::stop() {
    if(running_){
        slint::invoke_from_event_loop([] {slint::quit_event_loop;});
    }
}




//make slint timer to update every 50 milliseconds (20 Hz)



