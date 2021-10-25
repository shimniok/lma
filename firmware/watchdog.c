#include "config.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include "morse.h"

uint16_t seconds = 1;		// 2^32 = 4294967296 seconds = 7 weeks, way more than enough; int isn't enough
uint16_t warn_sec;		  // Delay (in seconds) before Warning start sounding
uint16_t sos_sec;				// Delay (in seconds) before SOS starts sounding
uint8_t pause = PERIOD;	// time to pause between SOS or Warning beeps

void initWatchdog(uint16_t warn, uint16_t sos)
{
	warn_sec = warn;
	sos_sec = sos;
}

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

#ifdef DEBUG
	dit();
#endif

	// Beep "W" if WARN time exceeded
	// Beep "SOS" if SOS time exceeded
	if (++pause >= PERIOD) {
		if (seconds >= sos_sec) {
			sos();
			pause = 0;
		} else if (seconds >= warn_sec) {
			w();
			pause = 0;
		}
	}

	// re-enable WDT interrupt
	enableWatchdog();

	return;
}
