#include <avr/wdt.h>

void disableWatchdog()
{
	// disable watchdog reset mode and interrupt mode
	WDTCR |= _BV(WDE) | _BV(WDCE);
	WDTCR &= ~_BV(WDE);
}

void enableWatchdog()
{
	// Enable watchdog interrupt, set prescaling to 1 sec
	WDTCR |= _BV(WDIE) | _BV(WDP2) | _BV(WDP1);
}
