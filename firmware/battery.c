#include "config.h"
#include "clock.h"
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
 * The lma is set up to feed a voltage divider with one pin (PB0), and read the divided
 * value with the other (PB3, ADC3). We have to use the internal voltage reference.
 * which varies from 1.0V to 1.2V. Also, the resistors in the divider are 5% tolerance.
 *
 * Calibration
 * -----------
 * That's a pretty huge error which requires calibration. Automatic calibration is
 * done when the lma first starts up (in initADC()) after its eeprom is erased
 * (set to 0).
 *
 * To calibrate, it reads the current voltage, which must be set to 5.0V (within 1%)
 * The adc value returned is 2x that of the 2.5V threshold.  So to get the 2.5V ADC
 * value, right shift the result, then store store it in the threshold eeprom location.
 *
 * Next time initADC() is called, it'll read the threshold and use that.
 *
 * Using
 * -----
 * To erase the calibration, just recompile, and program the eeprom with the .eep file
 * generated at compile time.
 *
 * Note that we want to preserve EEPROM so set Fuses EESAVE and BODLEVEL=1.8V
 */

EEMEM uint16_t cfg_threshold = 0;	// create .eep with this loc initialized to 0.
static uint16_t threshold = 0;      // threshold

uint8_t pinToMux(uint8_t pin) {
	uint8_t mux = 0;
	switch (pin) {
		case PB5: // ADC0
			mux = 0b0000;
			break;
		case PB2: // ADC1
			mux = 0b0001;
			break;
		case PB4: // ADC2
			mux = 0b0010;
			break;
		case PB3: // ADC3
			mux = 0b0011;
			break;
	}
	return mux;
}

void init_battery_thresh() {
	// Read the threshold ADC value for low battery warning
	threshold = eeprom_read_word(&cfg_threshold);

	// Auto-calibrate battery warning threshold.
	// Zero out threshold location in eeprom.
	// Then, start MCU with Vcc=5.0V
	if (threshold == 0) {
		// Get voltage (stored in volts)
		// Divide result by 2 to get 2.5V
		threshold = getVoltage() / 2;
		// Store the new threshold in eeprom
		eeprom_update_word(&cfg_threshold, threshold);
	}
}


void adc_on() {
	// internal 1.1V reference, select ADC pin
	ADMUX = (1<<REFS1)| 0b011; // TODO: convert to defines
	// Take ADC out of shutdown and clear left adjust register
	ADCSRA |= (1<<ADEN);
	ADCSRA &= ~(1<<ADLAR);
}

void adc_off() {
	// Disable ADC
	ADCSRA &= ~(1<<ADEN);
}

uint8_t ddrb;

void divider_on() {
	// enable voltage divider
	ddrb = DDRB;
	DDRB |= _BV(ADC_EN);
	PORTB |= _BV(ADC_EN);
}

void divider_off() {
	PORTB &= ~_BV(ADC_EN);
	DDRB = ddrb;
}

/**
 * Read battery voltage, compare to threshold value representing 2.5V which is
 * about where the CR1225 battery is at its end.
 */
uint16_t getVoltage()
{
	uint8_t i;

	divider_on();
	adc_on();

	// Read voltage, multiple samples w/ delay between each
	uint16_t sum = 0;
	for (i = 0; i < 16; i++) {
		// start conversion
		ADCSRA |= (1<<ADSC);
		// wait for result
		while ((ADCSRA & _BV(ADIF)) == 0x0);
		sum += ADCL;
		sum += ADCH<<8;
		// reset ADIF
		ADCSRA |= _BV(ADIF);
		// wait around for a little while
		wait_ms(10);
	}
	uint16_t avg = sum / 16;

	adc_off();

	// ADC = Vin * 1024 / Vref
	// Vin = (1024 / 1.1) / ADC result
	// x 1000 for 3 digits of precision
	uint16_t volts = (1100UL * avg) / 1024UL;

	divider_off();

	return volts;
}

/**
 * return the most recently read Voltage
 */
uint8_t checkVoltage() {
	return (getVoltage() > threshold);
}
