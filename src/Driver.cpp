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

        // для робота 1
        //mPidL.start(3.f, 0.000f, 0, 0, -50, 50);
        //mPidR.start(3.f, 0.000f, 0, 0, -50, 50);
        //mPidAngle.start(1.f, 0.0005f, 0, 0, -700, 700);
        //mPidRatio.start(2.f, 0.0f, 0, 0, -15, 15);
        // для калеки 5
        mPidL.start(25.f, 0.000f, 0, 0, -70, 70);
        mPidR.start(25.f, 0.000f, 0, 0, -70, 70);
        mPidAngle.start(8.f, 0.005f, 0, 0, -700, 700);
        mPidRatio.start(12.f, 0.0f, 0, 0, -25, 25);

        monitor.println("Driver enabled");
    });
}

void Driver::correct() {
    location.track();

    mDt = (float) deltaTimer1.pass() / 1000.f;
    mTime += mDt;

    // плавно ускоряем робота, ограничивая максимальную скорость
    if (mTime < accelerationTime) {
        mPidR.max(mTime / accelerationTime * maxSpeed);
        mPidL.max(mTime / accelerationTime * maxSpeed);
        mPidR.min(-mTime / accelerationTime * maxSpeed);
        mPidL.min(-mTime / accelerationTime * maxSpeed);
        mPidRatio.min(-mTime / accelerationTime * maxEngineDif - 5.f);
        mPidRatio.max(mTime / accelerationTime * maxEngineDif + 5.f);
    } else {
        mPidL.max(maxSpeed);
        mPidR.max(maxSpeed);
        mPidR.min(-maxSpeed);
        mPidL.min(-maxSpeed);
        mPidRatio.max(maxEngineDif);
        mPidRatio.min(-maxEngineDif);
    }

    // идеальное передвижение
    mSumL += mSpeedL * mDt;
    mSumR += mSpeedR * mDt;

    // фактическое передвижение
    mFactSumL += location.leftMovement();
    mFactSumR += location.rightMovement();

    mPidL.target(mSumL);
    mPidR.target(mSumR);

    auto l = mPidL.calculate(mFactSumL, mDt);
    auto r = mPidR.calculate(mFactSumR, mDt);

    mPidRatio.target(mSumR - mSumL);
    // пытаемся сделать отношения скоростей колёс постоянными
    auto ratioCorrect = mPidRatio.calculate(mFactSumR - mFactSumL, mDt);
    l -= ratioCorrect;
    r += ratioCorrect;

    monitor.println("Dt ", mDt, "A ", location.rotationDegrees(), " L", mFactSumL, "/", mSumL, "-",
            l, " R", mFactSumR, "/", mSumR,
            "-", r);
    fillL(l);
    fillR(r);
}

void Driver::rotate(float degrees) {
    speedL(0);
    speedR(0);
    degrees *= 1.25; // для калеки номер 5
    float const ac = accelerationTime;
    accelerationTime = 0;

    // пока угол поворота робота не достигнет нужного с допуском в 1 градус
    // продолжаем корректировать путь с помощью регулятора
    mPidAngle.target(degrees);
    while (!near(degrees, 0.1f, location.rotationDegrees())) {
        driver.correct();
        auto const a = mPidAngle.calculate(location.rotationDegrees(), mDt);
        // чтобы крутиться вокруг своей оси, даём одинаковые скорости в противоположные стороны
        driver.speedL(-a);
        driver.speedR(a);
    }
    stop();
    accelerationTime = ac;
}

void Driver::move(float len) {
    mSumL = len;
    mSumR = len;
    // ставим базовую скорость 0, чтобы регулятор сам решил, как нужно ехать
    speedL(0);
    speedR(0);
    // пока фактическая скорость робота не достигнет нужного расстояния
    // с погрешностью 1 миллиметр, продолжаем корректировать путь с помощью регулятора
    while (!near(mSumL, 1.f, mFactSumL) && !near(mSumL, 1.f, mFactSumL)) {
        correct();
    }
    stop();
}

Driver driver = {};