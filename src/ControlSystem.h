#include <avr/io.h>
#define F_CPU 16000000UL
#include "MUXController.h"
#include "Engine.h"

const float robotSpeed = 0.3;
const float robotKp = 0.001;

class SensorController
{
  public:
    SensorController(MUXController* mux, Engines* eng, ADC_CHANNEL adc1, ADC_CHANNEL adc2)
    {
      m_adc1 = adc1;
      m_adc2 = adc2;
      m_mux = mux;
    }

    void correct()
    {
      uint16_t s1 = m_mux->read(m_adc1);
      Serial.print("ADC0 : ");
      Serial.println(s1);
      uint16_t s2 = m_mux->read(m_adc2);
      Serial.print("ADC1 : ");
      Serial.println(s2);
      uint16_t cur = s1 - s2;

      int16_t err = 0 - cur;
      Serial.print("Err : ");
      Serial.println(err);
      float out = robotKp * err;

      Serial.print("Out : ");
      Serial.println(out);
      m_eng->setSpeed1(robotSpeed + out);
      m_eng->setSpeed2(robotSpeed - out);
    }

    private:
    uint16_t m_sp = 0;
    Engines* m_eng;
    ADC_CHANNEL m_adc1;
    ADC_CHANNEL m_adc2;
    MUXController* m_mux;
};
