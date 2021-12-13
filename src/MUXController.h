#include <avr/io.h>
#include <avr/interrupt.h>


enum ADC_CHANNEL
{
  C_ADC0,
  C_ADC1,
  C_ADC2,
  C_ADC3
};

enum DivisionFactor
{
  DF_2,
  DF_4,
  DF_8,
  DF_16,
  DF_32,
  DF_64,
  DF_128
};

volatile uint16_t adc_data = 0;
volatile uint16_t data_accepted = false;

class MUXController
{
  public:
    MUXController(DivisionFactor factor = DF_128)
    {
      // запускаем АЦП
      ADCSRA = 1 << ADEN;
      // запускаем преобразование
      ADCSRA |= 1 << ADSC;
      // запускаем непрерывные преобразования
      ADCSRA |= 1 << ADATE;

      // включаем прерывания
      ADCSRA |= 1 << ADIE;

      // опорное напряжение - напряжение питания AVCC
      ADMUX = 1 << REFS0;

      // задаём тактовую частоту
      switch(factor)
      {
        case DF_2:
        ADCSRA |= (1 << ADPS0);
        break;
        case DF_4:
        ADCSRA |= (1 << ADPS1);
        break;
        case DF_8:
        ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
        break;
        case DF_16:
        ADCSRA |= (1 << ADPS2);
        break;
        case DF_32:
        ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
        break;
        case DF_64:
        ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
        break;
        case DF_128:
        ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        break;
      }
    }

    uint16_t read(ADC_CHANNEL channel)
    {
      switch(channel)
      {
      case C_ADC0:
        ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
        break;
      case C_ADC1:
        ADMUX &= ~((1 << MUX1) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
        ADMUX |= (1 << MUX0);
        break;
      case C_ADC2:
        ADMUX &= ~((1 << MUX0) | (1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
        ADMUX |= (1 << MUX1);
        break;
      case C_ADC3:
        ADMUX &= ~((1 << MUX2) | (1 << MUX3) | (1 << MUX4) | (1 << MUX5));
        ADMUX |= (1 << MUX1) | (1 << MUX0);
        break;
      }
      while(!data_accepted);
      data_accepted = false;
      return adc_data;
    }
};
