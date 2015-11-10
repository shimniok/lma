#include "config.h"
#include <util/delay.h>
#include "morse.h"
#include "buzzer.h"

void sos()
{
	s(); o(); s(); space();
}

void k()
{
	dah(); dit(); dah(); space();
}

void o()
{
	dah(); dah(); dah(); space();
}

void s()
{
	dit(); dit(); dit(); space();
}

void w()
{
	dit(); dah(); dah(); space();
}

void number(uint8_t n) 
{
	// Beep out the current warning time
	while (n >= 10) {
		dah();
		n -= 10;
	}
	if (n > 0) {
		dit();
	}
}

void space()
{
	_delay_ms(DOT*2); // because we've already delayed 1x
}

void dit()
{
	beepOn();
	_delay_ms(DOT);
	beepOff();
	_delay_ms(DOT);
}

void dah()
{
	beepOn();
	_delay_ms(DOT*3);
	beepOff();
	_delay_ms(DOT);
}
