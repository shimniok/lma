#include <avr/interrupt.h>
#include <util/delay.h>
#include "clock.h"

volatile uint16_t millis = 0;

uint8_t get_cpu_prescaler()
{
	switch (F_PRESCALER) {
		case F_PRESCALER_1:
			return 1;
		case F_PRESCALER_2:
			return 2;
		case F_PRESCALER_4:
		  return 4;
		case F_PRESCALER_8:
		  return 8;
		case F_PRESCALER_16:
			return 16;
		case F_PRESCALER_32:
		  return 32;
		case F_PRESCALER_64:
		  return 64;
		case F_PRESCALER_128:
		  return 128;
		case F_PRESCALER_256:
		  return 256;
		default:
			return 1;
	}
}

void slowClock()
{
	CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	CLKPR = F_PRESCALER;
}

void wait_ms(uint16_t ms)
{
  for (int i=0; i < ms; i++)
    _delay_ms(1);
}
