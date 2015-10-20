/** Lost Model Alarm firmware
 * ATtiny25
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
 * In addition, the firmware provides a low battery
 * warning. See adc.c for details.
 *
 * Michael Shimniok - http://www.bot-thoughts.com
 */

#include "config.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h> 
#include <avr/eeprom.h>
#include "battery.h"
#include "buzzer.h"
#include "switch.h"
#include "morse.h"

EEMEM uint8_t cfg_rccal = 0;		 // EEPROM: RC oscillator calibration
EEMEM uint8_t cfg_warn_min = 5; // EEPROM: Delay before warning starts sounding, in minutes.

uint16_t warn_sec;				// Delay (in seconds) before Warning start sounding
uint16_t sos_sec;				// Delay (in seconds) before SOS starts sounding
uint16_t seconds = 1;		// 2^32 = 4294967296 seconds = 7 weeks, way more than enough; int isn't enough
uint8_t pause = PERIOD;	// time to pause between SOS or Warning beeps

void disableWatchdog();
void enableWatchdog();
void slowClock();

int main()
{
	cli();
	//wdt_disable();
	disableWatchdog();

	slowClock();
	initBuzzer();
	initSwitch();
	//initADC();
	
#ifdef DEBUG
	uint16_t v = getVoltage();
	uint16_t i;

	for (i = 0x8000; i != 0; i >>= 1) {
		if (v & i) {
			dit();
		} else {
			dah();
		}
		_delay_ms(1000);
	}
#endif
	
 	// Interrupt every second
	sei();

	// Retrieve the current warning timeout from eeprom
	uint8_t warn_min = eeprom_read_word(&cfg_warn_min);

	// If switch is depressed (at power up), begin increasing
	// warning time, in 5-minute increments, max 30 minutes
	// SOS time is 2 x warning time.
	//
	while (switchPressed()) {	
		// Increment warning time by 5 minutes
		warn_min += 5;

		// Maximum warning time is 30 minutes
		if (warn_min > 30) warn_min = 5;

		// Save the new warning time
		eeprom_update_word(&cfg_warn_min, warn_min);

		// Beep out the current warning time
		uint8_t i = warn_min;
		while (i) {
			if (i >= 10) {				// dah for each tens of minutes
				dah();
				i -= 10;
			} else if (i != 0) { // dit indicates 5 minutes
				dit();
				i = 0;
			}
			wdt_reset();
		}

		// pause between increments
		_delay_ms(3000);
		wdt_reset();
	}

	// Compute the number of seconds for warning and SOS
	warn_sec = warn_min * 60;
	sos_sec = warn_sec * 2;

	enableWatchdog();

	/*
	if (checkVoltage()) {
		ok();
	} else {
		sos();
	}
	*/

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
	//wdt_enable(WDTO_1S);

	return;
}

inline void disableWatchdog()
{
	// disable watchdog reset mode and interrupt mode
	MCUSR = 0x00;
	WDTCR |= _BV(WDE) | _BV(WDCE);
	WDTCR = 0x00;
}

inline void enableWatchdog()
{
	// Enable watchdog interrupt, set prescaling to 1 sec
	WDTCR |= _BV(WDIE) | _BV(WDP2) | _BV(WDP1);
}

// 8MHz / 64 = 125000
void slowClock()
{
	CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	CLKPR = _BV(CLKPS2) | _BV(CLKPS1); // scale = /64
}
