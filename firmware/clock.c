#include <avr/interrupt.h>

// 8MHz / 64 = 125000
void slowClock()
{
	CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	CLKPR = _BV(CLKPS2) | _BV(CLKPS1); // scale = /64
}
