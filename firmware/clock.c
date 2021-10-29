#include "config.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "clock.h"

uint16_t cpu_prescale[9] = { 1, 2, 4, 8, 16, 32, 64 };
uint16_t f_prescaler = 0;

uint16_t get_cpu_prescale()
{
	return cpu_prescale[f_prescaler];
}

void slowClock()
{
	CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	CLKPR = F_PRESCALER;
	f_prescaler = F_PRESCALER;
}

// TODO: fix bug in delay that happens before slowClock called

// delay routine that is aware of F_CLKDIV
void wait_ms(uint16_t ms)
{
	for (int j=0; j < F_CLKDIV; j++)
	  for (int i=0; i < ms; i++)
			_delay_ms(1);
}
