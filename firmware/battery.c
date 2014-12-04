#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "battery.h"

/** battery.c
 *
 * The lma warns the user on startup when the battery falls below ~2.5V
 * 
 * Circuit
 * -------
 * The lma is set up to feed a voltage divider with one pin, and read the divided
 * value with the other. We have to use the internal voltage reference. It varies
 * from 1.0V to 1.2V. Also, the resistors in the divider are 5% tolerance.
 * 
 * Calibration
 * -----------
 * That's a pretty huge error which requires calibration. Automatic calibration is
 * done when the lma first starts up (in initADC()) after its eeprom is erased 
 * (set to 0). 
 * 
 * To calibrate, it reads the current voltage, which we expect to be 5.0V (within 1%
 * hopefully).  The adc value returned is 2x that of the 2.5V threshold.  So to get
 * the 2.5V ADC value, right shift the result, then store store it in the threshold
 * eeprom location.
 * 
 * Next time initADC() is called, it'll read the threshold and use that.
 * 
 * Using
 * -----
 * To erase the calibration, just recompile, and program the eeprom with the .eep file
 * generated at compile time..
 * 
 * Note that we want to preserve EEPROM so set Fuses EESAVE and BODLEVEL=1.8V
 */


EEMEM uint16_t threshold = 0;	// create .eep with this loc initialized to 0.
static uint16_t t;				// threshold
static uint16_t volts;			// recently read voltage


void initADC() {
	ADMUX = (1<<REFS0)|(1<<MUX1)|(1<<MUX0);

	// Read the threshold ADC value for low battery warning
	t = eeprom_read_word(&threshold);

	// Auto-calibrate battery warning threshold.
	// Zero out threshold location in eeprom.
	// Then, start MCU with Vcc=5.0V
	if (t == 0) {
		// Get voltage (stored in volts)
		checkVoltage();
		// Divide result by 2 to get 2.5V
		t = volts >> 1;
		// Store the new threshold in eeprom
		eeprom_write_word(&threshold, t);
	}
}


/**
 * return the most recently read Voltage
 */
uint16_t getVoltage() {
	return volts;
}


/** 
 * Read battery voltage, compare to threshold value representing 2.5V which is
 * about where the CR1225 battery is at its end.
 */
uint8_t checkVoltage()
{
	uint8_t i;
	uint8_t volts_ok;

	// Take ADC out of shutdown
#if defined (__AVR_ATtiny13A__)
	PRR &= ~(1<<PRADC);
#endif
	// Enable ADC
	ADCSRA |= (1<<ADEN);
	ADCSRA &= ~(1<<ADLAR);
	// set bottom of voltage divider low
	PORTB &= ~(1<<PB4); 
	// set pin as output
	DDRB |= (1<<PB4);

	volts = 0;

	// Read voltage, multiple samples w/ delay between each
	for (i = 0; i < 32; i++) {
		// start conversion
		ADCSRA |= (1<<ADSC);
		// wait for result
		while ((ADCSRA & (1<<ADIF)) == 0x0);
		volts += ADCL;
		volts += ADCH<<8;
		//volts = ADCH;
		// reset ADIF
		ADCSRA |= (1<<ADIF);
		// wait around for a little while
		_delay_ms(5);
	}
	volts >>= 5; // average; divide by 32

	volts_ok = (volts > t);		

	// set pin as input, HiZ
	DDRB &= ~(1<<PB4);	
	// Disable ADC
	ADCSRA &= ~(1<<ADEN);
	// Shut down ADC
#if defined (__AVR_ATtiny13A__)
	PRR |= (1<<PRADC);
#endif
	return volts_ok;
}
