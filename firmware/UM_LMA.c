/** UM LMA - ultra micro lost model alarm firmware
 * Michael Shimniok http://www.bot-thoughts.com/
 */
 
#define F_CPU	150000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h> 
#include "adc.h"

//#define DEBUG		0

#define PERIOD		8			// Delay period between signal (seconds)
#define WARN_TIME   5*60		// Delay before warning stars sounding (seconds) every PERIOD seconds
#define SOS_TIME	10*60		// Delay before warning stops and SOS starts sounding (seconds) every PERIOD seconds

#define DOT			60			// length of dot in ms

#define BUZZ		0			// PB0
#define LED			2			// PB3

#define beepOn()		TCCR0B |= _BV(CS00);
#define beepOff()		TCCR0B &= ~_BV(CS00);
#define ledOn()			PORTB  |= _BV(LED);
#define ledOff()		PORTB  &= ~_BV(LED);

long seconds = 1;				// 2^32 = 4294967296 seconds = 7 weeks, way more than enough; int isn't enough
long pause = PERIOD;

void ok();
void sos();
void k();
void o();
void s();
void w();
void beepNum(uint8_t n);
void space();
void dit();
void dah();
void initBuzzer();
void initLED();
void disableWatchdog();
void enableWatchdog();
void slowClock();


/** ATtiny13 Lost Model Alarm sounds morse code "W" (warning) after a preset delay, and after a further
 * delay, "SOS" (mayday). The program utilizes sleep mode to conserve battery power and disables peripherals
 * between beeps.  The program is originally designed for an ultra-light LMA for ultra micro (UM) models like
 * the Hobbyzone Champ, inspired by losing mine to wind. (I got it back but still)
 *
 * Michael Shimniok - http://www.bot-thoughts.com
 */
int main(int argc, char **argv)
{
	cli();
	disableWatchdog();

	initBuzzer();
#ifdef DEBUG
	initLED();
#endif

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

void ok()
{
	ledOn();
	o(); k();
	ledOff();
}

void sos()
{
	s(); o(); s();
}

void k()
{
	dah(); dit(); dah(); space();
}

void o()
{
	dah(); dah(); dah(); space();
}

void s()
{
	dit(); dit(); dit(); space();
}

void w()
{
	dit(); dah(); dah(); space();
}

void beepDigit(uint8_t d) {
	uint8_t i;

	if (d == 0) {
		dah();
	} else if (d < 10) {
		for (i = 0; i < d; i++) {
			dit();
		}
	}
	space();
}

void beepNum(uint8_t n)
{
	uint8_t a, b;

//	a = n / 10U;
//	b = n - a * 10U;

	beepDigit(a);
	beepDigit(b);
}

void space()
{
	_delay_ms(DOT*2); // because we've already delayed 1x
}

void dit()
{
#ifdef DEBUG 
	ledOn();
#endif
	beepOn();
	_delay_ms(DOT);
#ifdef DEBUG 
	ledOff();
#endif
	beepOff();
	_delay_ms(DOT);
}

void dah()
{
#ifdef DEBUG 
	ledOn();
#endif
	beepOn();
	_delay_ms(DOT*3);
#ifdef DEBUG 
	ledOff();
#endif
	beepOff();
	_delay_ms(DOT);
}


void initBuzzer()
{
    // Need 3.8kHz tone, F_CPU/2/3800
	// set freq with output compare register
    // Want to toggle OC0A on Compare Match: COM0A1=0 (TCCR0A:7), COM0A0=1 (TCCR0A:6) for non-PWM (CTC) mode
    // CTC mode -- clear on timer compare: WGM02=0 WGM01=1 WGM00=0
    // Prescale /1: CS02=0 CS01=0 CS00=1
    TCNT0=0;
    OCR0A  = F_CPU/2/3800;
    TCCR0A = _BV(COM0A0) | _BV(WGM01);

	// Set LED and Buzzer as output
	DDRB |= _BV(BUZZ);
}

void initLED()
{
	DDRB |= _BV(LED);
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
