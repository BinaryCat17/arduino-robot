#include <HardwareSerial.h>
#include "Driver.hpp"
#include <avr/interrupt.h>

int main()
{
  cli();
  Serial.begin(9600);
  Driver::enable();
  Driver::speed(0.3f);
  sei();
  
  while(true)
  {
      Driver::correct();
  }
}