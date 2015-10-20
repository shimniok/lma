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

EEMEM uint16_t cfg_warn =  5*60; // Delay before warning starts sounding (seconds) every PERIOD seconds
EEMEM uint16_t cfg_sos  = 10*60; // Delay before warning stops and SOS starts sounding (seconds) every PERIOD seconds

uint16_t warn_time;
uint16_t sos_time;
uint16_t seconds = 1;				// 2^32 = 4294967296 seconds = 7 weeks, way more than enough; int isn't enough
uint16_t pause = PERIOD;

//void disableWatchdog();
//void enableWatchdog();
void slowClock();

int main(int argc, char **argv)
{
	cli();
	wdt_disable();

//	warn_time = eeprom_read_word(&cfg_warn);
//	sos_time = eeprom_read_word(&cfg_sos);

	slowClock();

	initBuzzer();
	/*
	initADC();
	initSwitch();
	
	if (checkVoltage()) {
		ok();
	} else {
		sos();
	}

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
	
	wdt_enable(WDTO_1S);

 	sei();

	int i;
	for(i = 0; i < 10; i++) {
		number(2);
		_delay_ms(3000);
	}

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
		if (seconds >= sos_time) {
			sos();
			pause = 0;
		} else if (seconds >= warn_time) {
			w();
			pause = 0;
		}
	}

	// re-enable WDT interrupt
	wdt_enable(WDTO_1S);

	return;
}

/*
void disableWatchdog()
{
	_WDR();
	// disable watchdog reset mode and interrupt mode
	MCUSR = 0x00;
	WDTCR |= _BV(WDE) | _BV(WDCE);
	WDTCR = 0x00;
}

void enableWatchdog()
{
	// Enable watchdog interrupt, set prescaling to 1 sec
	WDTCR |= _BV(WDIE) | _BV(WDP2) | _BV(WDP1);
}
*/

// 8MHz / 64 = 125000
void slowClock()
{
	CLKPR = _BV(CLKPCE); // enable change to clock prescaler
	CLKPR = _BV(CLKPS2) | _BV(CLKPS1); // scale = /64
}
