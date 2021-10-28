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

EEMEM uint8_t cfg_rccal = 0;		 // EEPROM: RC oscillator calibration
EEMEM uint8_t cfg_warn_min = 5;  // EEPROM: Delay before warning starts sounding, in minutes.
uint16_t warn_sec;		           // Delay (in seconds) before Warning start sounding
uint16_t sos_sec;				         // Delay (in seconds) before SOS starts sounding
uint8_t pause = PERIOD;          // time to pause between SOS or Warning beeps

int main()
{
	disableWatchdog();
	sei();

	initSwitch();

	wait_ms(2000);
	slowClock();

	init_battery_thresh();

	// Retrieve the current warning timeout from eeprom
	uint8_t warn_min = eeprom_read_byte(&cfg_warn_min);

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
		wait_ms(3000);
	}

	/*
	#ifdef DEBUG
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

	// set warn_sec, sos_sec
	sos_sec = 10;
	warn_sec = 5;

	enableWatchdog();

	uint16_t pause = 0;
	uint16_t period = PERIOD;

	while (1) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_mode();
		if (++pause >= period) {
			if (seconds >= sos_sec) {
				message(SOS);
				pause = 0;
			} else if (seconds >= warn_sec) {
				message(WARNING);
				pause = 0;
			}
			/*
			if (checkVoltage() == 0) {
				period = PERIOD_LOW;
			}
			*/
		}
		enableWatchdog();
	}
}


	// Beep "W" if WARN time exceeded
	// Beep "SOS" if SOS time exceeded
	/*
	if (++pause >= PERIOD) {
		if (seconds >= sos_sec) {
			sos();
			pause = 0;
		} else if (seconds >= warn_sec) {
			w();
			pause = 0;
		}
	}
	*/

	// re-enable WDT interrupt
	//enableWatchdog();
