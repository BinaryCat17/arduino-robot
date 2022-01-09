#include "App.hpp"
#include "SerialMonitor.hpp"
#include "avr/interrupt.h"

namespace AvrLib {
    Main _implUserApp = {};
}

int main() {
#if defined(ENABLE_DEBUG)
    debug_init();
#endif

#if defined(ENABLE_SERIAL) and not defined(ENABLE_DEBUG)
    Serial.begin(9600);
#endif

    AvrLib::monitor.println("Starting...");

    AvrLib::_implUserApp.setup();
    sei();

    while (true) {
        AvrLib::_implUserApp.loop();
#if defined(ENABLE_DEBUG)
        if (serialEventRun) serialEventRun();
#endif
    }
}