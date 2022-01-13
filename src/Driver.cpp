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
    mPidAngle.target(degrees);
    while (!near(degrees, 1.f, location.rotationDegrees()))
    {
        driver.correct();
        auto const a = mPidAngle.calculate(location.rotationDegrees(), mDt);
        monitor.println(location.rotationDegrees(), " ", mDt, " ", a);
        driver.speedL(-a);
        driver.speedR(a);
    }
    monitor.println("Hello");
    driver.stop();
}

Driver driver = {};