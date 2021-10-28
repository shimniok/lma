#include "config.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "morse.h"

uint16_t seconds = 0;

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

ISR(WDT_vect)
{
	seconds++;	// increment 'clock'
}
