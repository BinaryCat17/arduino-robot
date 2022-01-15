#include "App.hpp"
#include "Runner.hpp"

void Main::setup() {
    runner.enable();
}

void Main::loop() {
    runner.run();
    Position const pos = location.pos();
    monitor.println("X ", pos.x, " Y ", pos.y);
}