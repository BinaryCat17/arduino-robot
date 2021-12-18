#include "Driver.hpp"
#include "Timer.hpp"
#include "PinMap.hpp"
#include "App.hpp"

class RobotApp : public CustomApp {
public:
    void init() override {
        // настраиваем диод на выход
        PinMap::direction(Pin::D13, PinDir::Out);

        // мигаем диодом раз в пол секунды - подаём признаки жизни
        lightOn.start(1000, 0);
        lightOff.start(1000, 500);

        // базовая скорость двигателей
        Driver::speed(0.25);
        Driver::enable();
    }

    void loop() override {
        // подаём признаки жизни
        if (lightOn.event()) {
            PinMap::write(Pin::D13, PinValue::High);
        }

        if (lightOff.event()) {
            PinMap::write(Pin::D13, PinValue::Low);
        }

        Driver::correct();
    }

private:
    Timer lightOn;
    Timer lightOff;
};

int main() {
    RobotApp app;
    App::start(&app);
}