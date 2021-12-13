#include "ControlSystem.h"

int main()
{
  cli();
  Serial.begin(9600);
  MUXController mux;
  Engines engines;
  SensorController controller(&mux, &engines, C_ADC0, C_ADC1);

  sei();
  
  while(true)
  {
    //Serial.print(mux.read(C_ADC0));
    Serial.print(" ");
    //Serial.println(mux.read(C_ADC1));

    controller.correct();
  }
}

ISR(ADC_vect)
{
  adc_data = ADCL | (ADCH << 8);
  data_accepted = true;
}
