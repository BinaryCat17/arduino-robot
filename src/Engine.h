#include <avr/io.h>
#define F_CPU 16000000UL

const uint16_t top_value = 3199;

class Engines
{
  public:
  Engine()
  {
  
    // настройка ножек на выход
    DDRE |= (1 << PE4) | (1 << PE5);

    // выбираем режим работы таймера Fast PWM (TOP = ICR3)
    TCCR3A = (1 << WGM31);
    TCCR3B = (1 << WGM32) | (1 << WGM33);

    // неинвертированный режим работы на каналах B и C
    TCCR3A |= (1 << COM3B1) | (1 << COM3C1);

    // настраиваем предделитель (prescaling) clk/1
    TCCR3B |= 1 << CS30;

    // записываем TOP value, таймер считывает от 0 до TOP 
    ICR3 = top_value;

    // заполнение ШИМ (duty cycle) (от 0 до TOP)
    OCR3B = 500; // конэффициент заполнения 15%
    OCR3C = 2000; 
  }

  void setSpeed1(float val)
  {
    //OCR3B = top_value * (1 - val);
  }

  void setSpeed2(float val)
  {
    //OCR3C = top_value * (1 - val);
  }
};
