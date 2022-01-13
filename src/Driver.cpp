#include "Driver.hpp"

void Driver::enable() {
    enableOnce([=] {
        deltaTimer1.enable(Precision::Micros);

        pinDirection<PinOut, DPin::D49>();
        pinDirection<PinOut, DPin::D51>();
        pinDirection<PinOut, DPin::D2>();
        pinDirection<PinOut, DPin::D3>();

        timerPWM3.freq<5000>();
        timerPWM3.mode2<PWMOutMode::NonInverting>();
        timerPWM3.mode3<PWMOutMode::NonInverting>();
        timerPWM3.enable();

        location.enable();

        mPidL.start(0.8f, 0.0000, 0, 0, -50, 50);
        mPidR.start(0.8f, 0.0000, 0, 0, -50, 50);
        mPidAngle.start(8.f, 0.1, 0, 0, -700, 700);

        monitor.println("Driver enabled");
    });
}

void Driver::correct() {
    location.track();

    mDt = (float) deltaTimer1.pass() / 1000.f;

    // идеальное передвижение
    mSumL += mSpeedL * mDt;
    mSumR += mSpeedR * mDt;

    // фактическое передвижение
    mFactSumL += location.leftMovement();
    mFactSumR += location.rightMovement();

    mPidL.target(mSumL);
    mPidR.target(mSumR);

    auto const l = mPidL.calculate(mFactSumL, mDt);
    auto const r = mPidR.calculate(mFactSumR, mDt);

    monitor.println("Dt ", mDt, " L",  mFactSumL, "/", mSumL, "-", l, " R",  mFactSumR, "/", mSumR, "-", r);
    fillL(l);
    fillR(r);
}

void Driver::rotate(float degrees) {
    degrees *= 1.08;

    // пока угол поворота робота не достигнет нужного с допуском в 1 градус
    // продолжаем корректировать путь с помощью регулятора
    mPidAngle.target(degrees);
    while (!near(degrees, 1.f, location.rotationDegrees()))
    {
        driver.correct();
        auto const a = mPidAngle.calculate(location.rotationDegrees(), mDt);
        monitor.println(location.rotationDegrees(), " ", mDt, " ", a);
        // чтобы крутиться вокруг своей оси, даём одинаковые скорости в противоположные стороны
        driver.speedL(-a);
        driver.speedR(a);
    }
    monitor.println("Hello");
    driver.stop();
}

void Driver::move(float len) {
    mSumL = len;
    mSumR = len;
    // ставим базовую скорость 0, чтобы регулятор сам решил, как нужно ехать
    speedL(0);
    speedR(0);

    // пока фактическая скорость робота не достигнет нужного расстояния
    // с погрешностью 1 миллиметр, продолжаем корректировать путь с помощью регулятора
    while(!near(mSumL, 1.f, mFactSumL) && !near(mSumL, 1.f, mFactSumL))
    {
        correct();
    }
    stop();
}

Driver driver = {};