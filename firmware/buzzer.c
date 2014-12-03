#include "config.h"
#include <avr/io.h>
#include "buzzer.h"

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


void beepOn()
{
	TCCR0B |= _BV(CS00);
}

void beepOff()
{
	TCCR0B &= ~_BV(CS00);
}
