#include <avr/interrupt.h>
#include <util/delay.h>
#include "clock.h"

volatile uint16_t millis = 0;

void slowClock()
{
	//CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	//CLKPR = _BV(CLKPS2); // scale = /16
}

void wait_ms(uint16_t ms)
{
  for (int i=0; i < ms; i++)
    _delay_ms(1);
}
