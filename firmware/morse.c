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

void number(int n)
{
	switch (n) {
	case 0:
		dah(); dah();	dah(); dah(); dah(); space();
		break;
	case 1:
		dit(); dah();	dah(); dah(); dah(); space();
		break;
	case 2:
		dit(); dit();	dah(); dah(); dah(); space();
		break;
	case 3:
		dit(); dit();	dit(); dah(); dah(); space();
		break;
	case 4:
		dit(); dit(); dit(); dit(); dah(); space();
		break;
	case 5:
		dit(); dit(); dit(); dit(); dit(); space();
		break;
	case 6:
		dah(); dit(); dit(); dit(); dit(); space();
		break;
	case 7:
		dah(); dah(); dit(); dit(); dit(); space();
		break;
	case 8:
		dah(); dah(); dah(); dit(); dit(); space();
		break;
	case 9:
		dah(); dah(); dah(); dah(); dit(); space();
		break;
	default:
		break;
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
