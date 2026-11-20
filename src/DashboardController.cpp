#include "dashboard_UI.h"
#include "../include/DashboardController.h"
#include <slint.h>

DashboardUI::DashboardUI(VehicleDataRingBuffer &buffer) : buffer_(buffer) {}

void DashboardUI::run() {
    auto dashboard = dashboard_Basic::create();
    dashboard->set_screen_res_w(Constants::screen_res_w);
    dashboard->set_screen_res_h(Constants::screen_res_h);
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
