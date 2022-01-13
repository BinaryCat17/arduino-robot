#include "App.hpp"
#include "DistanceSensor.hpp"
#include "Driver.hpp"

void Main::setup() {
    location.enable();
    distanceSensor.enable();
    driver.enable();

    //driver.fillL(100);
    //driver.fillR(100);
    driver.move(1000);
    //driver.rotate(90);
    //driver.rotate(0);
    //driver.rotate(-90);
    //driver.speedL(0);
    //driver.speedR(0);
    //driver.rotate(0);
    //driver.rotate(-90);
}

void Main::loop() {
    //driver.correct();
    float const angle = location.rotationRadians() * 180 / (2 * 3.1415926535f);
    Position const pos = location.pos();
    monitor.println("L ", pos.x, " R ", pos.y, " D", distanceSensor.distanceMm(), " mm ", "A",
            angle);
}