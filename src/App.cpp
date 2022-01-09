#include "App.hpp"
#include "DistanceSensor.hpp"

void Main::setup() {
    location.enable();
    distanceSensor.enable();
}

void Main::loop() {
    Position pos = location.pos();
    monitor.print("L ", pos.x);
    monitor.println(" R ", pos.y, " mm");


}

