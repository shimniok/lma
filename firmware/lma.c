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
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "battery.h"
#include "buzzer.h"
#include "clock.h"
#include "morse.h"
#include "switch.h"
#include "watchdog.h"

EEMEM uint8_t cfg_rccal = 0;		 // EEPROM: RC oscillator calibration
EEMEM uint8_t cfg_warn_min = 5;  // EEPROM: Delay before warning starts sounding, in minutes.

int main()
{
	cli();
	disableWatchdog();

	initSwitch();
	initBuzzer();
	initADC();

	/* enable programming, prevent slowClock(), press button */
	while (switchPressed());

	slowClock();
	dit();
	_delay_ms(2000);

/*
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
*/

	// Retrieve the current warning timeout from eeprom
	uint8_t warn_min = eeprom_read_byte(&cfg_warn_min);

	if (switchPressed()) {
		number(warn_min);
		// pause between increments
		_delay_ms(3000);
	}

	// If switch is depressed (at power up), begin increasing
	// warning time, in 5-minute increments, max 30 minutes
	// SOS time is 2 x warning time.
	//
	while (switchPressed()) {

		if (switchPressed()) {
			// Increment warning time by 5 minutes
			warn_min += 5;

			// Maximum warning time is 30 minutes
			if (warn_min > 30) warn_min = 5;

			// Save the new warning time
			eeprom_update_byte(&cfg_warn_min, warn_min);
		}
		number(warn_min);

		// pause between increments
		_delay_ms(3000);
	}

	// Compute the number of seconds for warning and SOS
	initWatchdog(warn_min * 60, warn_min * 60 * 2);

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
