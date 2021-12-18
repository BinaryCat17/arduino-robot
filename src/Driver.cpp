#include "Driver.hpp"

#include "MUXController.hpp"
#include "PWMGen.hpp"
#include "App.hpp"

const float robotKp = 0.0005;

void Driver::enable() {
    MUXController::enable();
    PWMGen::enable(PWMTimer::T3P235, 5000);

    App::println("Driver enabled");
    PinMap::direction(Pin::D49, PinDir::Out);
    PinMap::direction(Pin::D51, PinDir::Out);

    // пробный рывок
    PinMap::write(Pin::D49, PinValue::High);
    PinMap::write(Pin::D51, PinValue::High);
}

void Driver::speed(float speed) {
    instance().m_robotSpeed = speed;
}

Driver &Driver::instance() {
    static Driver driver;
    return driver;
}

void Driver::correct() {
    auto s1 = (int16_t) MUXController::read(Pin::A0);
    App::print("ADC0 : ");
    App::println(s1);

    auto s2 = (int16_t) MUXController::read(Pin::A1);
    App::print("ADC1 : ");
    App::println(s2);

    // регулятор
    int16_t cur = s1 - s2;
    int16_t err = 0 - cur;

    App::print("Err : ");
    App::println(err);
    float out = robotKp * err;

    // если выехали на белое, то выполняем поворт, пока не найдём путь
    if (s1 < 250 && s2 < 250) {
        // счетчик нужен, чтобы плавно ускорять поворот робота при выезде на белое
        // помогает плавнее пересекать острые линии и кривую
        ++instance().cnt;
        // если робот перед выездом на белую поверхность пересекал черный слева чаще,
        // то нужно поворачивать налево - так можно пересечь острые углы
        if (instance().sum > 0) {
            speed1((-instance().m_robotSpeed - out * (float) instance().cnt / 3));
            speed2((instance().m_robotSpeed + out * (float) instance().cnt / 3));
        } else {
            // действуем похожим образом, только для права
            speed1((instance().m_robotSpeed + out * (float) instance().cnt/ 3));
            speed2((-instance().m_robotSpeed - out * (float) instance().cnt / 3));
        }
    } else {
        // нормальная езда
        instance().sum += err;
        instance().cnt = 0;
        speed1((instance().m_robotSpeed - out));
        speed2((instance().m_robotSpeed + out));
    }
}

void Driver::engineSpeed(float speed, Pin engine, Pin control) {
    // проверяем на выход из допустимого диапазона
    if (speed > 1.0) {
        speed = 1.0;
    } else if (speed < -1.0) {
        speed = -1.0;
    }

    if (speed > 0) {
        // крутим вперёд
        PWMGen::fillFactor(engine, speed);
        PinMap::write(control, PinValue::Low);
    } else {
        // нужно инвертировать значение, если крутим колесом назад
        PWMGen::fillFactor(engine, 1.0f + speed);
        PinMap::write(control, PinValue::High);
    }
}
