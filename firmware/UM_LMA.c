/** ATtiny13(A) Lost Model Alarm firmware
 *
 * Sounds morse code "W" (warning) after a preset
 * delay, and after a further delay, "SOS" (mayday). 
 * The program utilizes sleep mode to conserve battery
 * power and disables peripherals between beeps.  The 
 * program is originally designed for an ultra-light
 * LMA for ultra micro (UM) models like the Hobbyzone
 * Champ, inspired by losing mine to wind. (I got it
 * back but still...)
 *
 * Michael Shimniok - http://www.bot-thoughts.com
 */

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h> 
#include "adc.h"
#include "buzzer.h"
#include "morse.h"

long seconds = 1;				// 2^32 = 4294967296 seconds = 7 weeks, way more than enough; int isn't enough
long pause = PERIOD;

void disableWatchdog();
void enableWatchdog();
void slowClock();

int main(int argc, char **argv)
{
	cli();
	disableWatchdog();

	initBuzzer();

	slowClock();

	initADC();

	if (checkVoltage()) {
		ok();
	} else {
		sos();
	}

	enableWatchdog();

 	sei();

	while (1) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
	}
}


ISR(WDT_vect)
{
	seconds++;	// increment 'clock'

#ifdef DEBUG 
	dit();
#endif

	// Beep "W" if WARN time exceeded
	// Beep "SOS" if SOS time exceeded
	if (seconds >= SOS_TIME) {
		if (++pause >= PERIOD) {
			sos();
			pause = 0;
		}
	} else if (seconds >= WARN_TIME) {
		if (++pause >= PERIOD) {
			w();
			pause = 0;
		}
	}

    //ledOff();
	//beepOff();

	// re-enable WDT interrupt
	WDTCR |= _BV(WDTIE);

	return;
}


void disableWatchdog()
{
	// disable watchdog reset mode and interrupt mode
	MCUSR &= ~_BV(WDRF);
	WDTCR |= _BV(WDE) | _BV(WDCE);
	WDTCR = 0x00;
}

void enableWatchdog()
{
	// Enable watchdog interrupt, set prescaling to 1 sec
	WDTCR |= _BV(WDTIE) | _BV(WDP2) | _BV(WDP1);
}


// 150kHz
void slowClock()
{
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS2) | _BV(CLKPS1); // scale = /64
}
