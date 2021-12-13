#include <HardwareSerial.h>
#include "Driver.hpp"

int main()
{
  cli();
  Serial.begin(9600);
  Driver::enable();
  sei();
  
  while(true)
  {
      Driver::correct();
  }
}