#include "Driver.hpp"
#include "Timer.hpp"
#include "PinMap.hpp"
#include "App.hpp"

class RobotApp {
public:
    void init() {
        HardwareTimer::enable();

        // настраиваем диод на выход
        PinMap::direction<PinOut, Pin::D13>();

        // мигаем диодом раз в пол секунды
        lightOn.start(500, 0);
        lightOff.start(500, 250);

        // базовая скорость двигателей
        Driver::enable();
        Driver::speed(300);
    }

    void loop() {
        // подаём признаки жизни
        if (lightOn.event()) {
            PinMap::write<PinHigh, Pin::D13>();
        }

        if (lightOff.event()) {
            PinMap::write<PinLow, Pin::D13>();
        }

        Driver::drive();
    }

private:
    Timer lightOn;
    Timer lightOff;
};

int main() {
    App::start<RobotApp>();
}