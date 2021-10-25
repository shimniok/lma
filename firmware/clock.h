#ifndef __clock_h
#define __clock_h

#define F_PRESCALER 1

#include <avr/io.h>

void wait_ms(uint16_t ms);
void slowClock();

#endif
