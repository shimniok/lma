#include <avr/io.h>
#include <util/delay.h> 
#include "config.h"
#include "adc.h"

static uint16_t volts;					// global voltage x 10  e.g. 3.7v => 37

void initADC()
{
	// 1.1V ref, PB3
	ADMUX = _BV(REFS0) | _BV(MUX1) | _BV(MUX0);

}

/** use onboard ADC to read voltage of the voltage divider versus 1.1V reference;
 * Voltage divider features two resistors, 220K and 100K giving 0.9375V with VIN=3V
 */
uint16_t readVoltage()
{
	int8_t i;

	// Take ADC out of shutdown
	// PRR &= ~_BV(PRADC);
	// Enable ADC
	ADCSRA |= _BV(ADEN);
	// set bottom of voltage divider low
	PORTB &= ~_BV(PB4); 
	// set pin as output
	DDRB |= _BV(PB4);	

	// reset volts variable
	volts = 0;

	// Read voltage, multiple samples w/ delay between each
	for (i=0; i < 32; i++) {
		// start conversion
		ADCSRA |= _BV(ADSC);
		// wait for result
		while ((ADCSRA & _BV(ADIF)) == 0x0);
		volts += ADCL;
		volts += (ADCH<<8);
		// reset ADIF
		ADCSRA |= _BV(ADIF);
		// wait around for a little while
		_delay_ms(20);
	}
	volts >>= 5;

	// Assumes 220K / 100K voltage divider, 1.1V internal ref.
	// Gives m=291, b=-0.68, so we use m=29 and b=-7
	volts -= 7;
	volts /= 29;

	// set pin as input, HiZ
	DDRB &= ~_BV(PB4);	
	// Disable ADC
	ADCSRA &= ~_BV(ADEN);
	// TODO: Shut down ADC
	// PRR |= _BV(PRADC);

	return volts;
}


uint16_t getVolts()
{
	return volts;
}
