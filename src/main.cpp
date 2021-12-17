#include "Driver.hpp"
#include "Timer.hpp"
#include "PinMap.hpp"
#include "Arduino.h"

#define ENABLE_DEBUG

#include "App.hpp"

class RobotApp : public CustomApp {
public:
    void init() override {

        light.interval(1000);
        light.start();

        drive.interval(1);
        drive.start();

        PinMap::direction(Pin::D13, PinDir::Out);
        App::println("Starting");

        Driver::speed(0.25);
        Driver::enable();
        Driver::calibrate();
    }

    void loop() override {
        // подаём признаки жизни
        if (light.event()) {
            if (flag) {
                flag = false;
                PinMap::write(Pin::D13, PinValue::High);
                App::println("light on");
            } else {
                flag = true;
                PinMap::write(Pin::D13, PinValue::Low);
                App::println("light off");
            }
        }


        if (drive.event()) {
            // едем
            Driver::correct();
        }
    }

private:
    Timer light;
    Timer drive;
    bool flag = false;
};

int main() {
    RobotApp app;
    App::start(&app);
}