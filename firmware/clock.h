#ifndef __clock_h
#define __clock_h

#include <avr/io.h>

#define F_PRESCALER_1    0
#define F_PRESCALER_2    _BV(CLKPS0)
#define F_PRESCALER_4    _BV(CLKPS1)
#define F_PRESCALER_8    (_BV(CLKPS1)|_BV(CLKPS0))
#define F_PRESCALER_16   _BV(CLKPS2)
#define F_PRESCALER_32   (_BV(CLKPS2)|_BV(CLKPS0))
#define F_PRESCALER_64   (_BV(CLKPS2)|_BV(CLKPS1))
#define F_PRESCALER_128  (_BV(CLKPS2)|_BV(CLKPS1)|_BV(CLKPS0))
#define F_PRESCALER_256  _BV(CLKPS3)

#define F_PRESCALER F_PRESCALER_1

uint16_t get_cpu_prescale();
void wait_ms(uint16_t ms);
void slowClock();

#endif
