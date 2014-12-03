#include <avr/delay.h>
#include "morse.h"

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

void space()
{
	_delay_ms(DOT*2); // because we've already delayed 1x
}
