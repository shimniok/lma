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
#include "clock.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include "battery.h"
#include "buzzer.h"
#include "morse.h"
#include "switch.h"
#include "watchdog.h"

#define after(x) (seconds >= (x))

EEMEM uint8_t cfg_rccal = 0;		 // EEPROM: RC oscillator calibration
EEMEM uint8_t cfg_warn_min = 5;  // EEPROM: Delay before warning starts sounding, in minutes.
uint8_t warn_min = 0; // minutes before warning beeps begin


void config() {
	// Retrieve the current warning timeout from eeprom
	warn_min = eeprom_read_byte(&cfg_warn_min);

	// If switch is depressed (at power up), begin increasing
	// warning time, in 5-minute increments, max 30 minutes
	// SOS time is 2 x warning time.
	//
	while (switch_pressed()) {
		if (switch_pressed()) {
			// Increment warning time by 5 minutes
			warn_min += 5;
			// Maximum warning time is 30 minutes
			if (warn_min > 30) warn_min = 5;
			// Save the new warning time
			eeprom_update_byte(&cfg_warn_min, warn_min);
		}
		number(warn_min);
		// pause between increments
		wait_ms(3000);
	}
}


void test_battery() {
	/*
#ifdef DEBUG
	// beep out voltage in binary, 1 == dah, 0 == dit
	uint16_t v = getVoltage();
	uint16_t i;

	for (i = 0x8000; i != 0; i >>= 1) {
		if (v & i) {
			dah();
		} else {
			dit();
		}
		wait_ms(1000);
	}
	wait_ms(2000);
#endif
	*/

	if (checkVoltage()) {
		message(OK);
	} else {
		message(SOS);
	}
}


void loop() {
	uint16_t next_warn_sec = 0;                // Keeps track of next time to warn/sos
	uint16_t warn_period = WARN_PERIOD;        // Warn every WARN_PERIOD (changed to WARN_PERIOD_LOW when batt low)
	//uint16_t next_batt_sec = BATT_PERIOD;      // keeps track of next time to check battery
	uint16_t next_batt_sec = 30;
	// uint16_t warn_begin_sec = warn_min*60;     // Delay (in seconds) before Warning start sounding
	uint16_t warn_begin_sec = 10;
	uint16_t sos_begin_sec = warn_begin_sec*2; // Delay (in seconds) before SOS starts sounding
	char *msg = NULL;

	while (1) {
		enableWatchdog(); // (re-)enable watchdog
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();

		// check battery voltage periodically
		if (after(next_batt_sec)) {
			message("-...");
			if (!checkVoltage()) {
				warn_period = WARN_PERIOD_LOW; // if batt low, increase warning period if battery low
				// TODO: prevent further battery checks
			}
			next_batt_sec += BATT_PERIOD;  // check again in BATT_PERIOD seconds
		}

		// Change msg after warning time or sos time exceeded
		if (!msg) {
			if (after(sos_begin_sec)) {
				msg = SOS;
			} else if (after(warn_begin_sec)) {
				msg = WARNING;
			}
		} else {
			// send message if warn_period exceeded
			if (after(next_warn_sec)) {
				next_warn_sec += warn_period;
				message(msg);
			}
		}

	}
}


int main() {
	disableWatchdog();
	sei();

	init_switch();

	init_battery_thresh();

//	wait_ms(2000); // just in case slower clock screws up programming...
	slowClock();

	config();
	test_battery();

	loop();
}
