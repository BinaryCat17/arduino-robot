#include "Driver.hpp"
#include "Timer.hpp"
#include "PinMap.hpp"
#include "Arduino.h"

#define ENABLE_DEBUG

#include "App.hpp"

class RobotApp : public CustomApp {
public:
    void init() override {
        App::println("Starting");

        // настраиваем диод на выход
        PinMap::direction(Pin::D13, PinDir::Out);

        // мигаем диодом раз в пол секунды
        lightOn.start(1000, 0);
        lightOff.start(1000, 500);

        // раз в 10 мс запускаем регулятор
        drive.start(10);

        // базовая скорость двигателей
        Driver::speed(0.25);
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
    Timer lightOn;
    Timer lightOff;
    Timer drive;
};

int main() {
    RobotApp app;
    App::start(&app);
}