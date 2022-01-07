#include "PinInputHandler.hpp"
#include "avr/interrupt.h"

class Main;

ISR(INT0_vect) {
    AvrLib::PinHandlerInterrupts<Main>::handle21();
}

ISR(INT1_vect) {
    AvrLib::PinHandlerInterrupts<Main>::handle20();
}

ISR(INT2_vect) {
    AvrLib::PinHandlerInterrupts<Main>::handle19();
}

ISR(INT3_vect) {
    AvrLib::PinHandlerInterrupts<Main>::handle18();
}

namespace AvrLib
{
    PinInputHandler pinInputHandler = {};
}