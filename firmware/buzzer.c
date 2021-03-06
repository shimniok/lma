#include "config.h"
#include <avr/io.h>
#include "buzzer.h"

inline void initBuzzer()
{
    // Need 3.8kHz tone, 150kHz/2/3800
    // set freq with output compare register
    // Prescale /1: CS02=0 CS01=0 CS00=1
    TCNT0=0;
    OCR0A  = F_CPU/2/3800;

    // Set LED and Buzzer as output
    DDRB |= _BV(BUZZ);
}


inline void beepOn()
{
    // Want to toggle OC0A on Compare Match: COM0A1=0 (TCCR0A:7), COM0A0=1 (TCCR0A:6) for non-PWM (CTC) mode
    // CTC mode -- clear on timer compare: WGM02=0 WGM01=1 WGM00=0
    TCCR0A = _BV(COM0A0) | _BV(WGM01); 
    TCCR0B = _BV(CS00); // no clock prescaling
}

inline void beepOff()
{
    // disable timer, clock
    TCCR0A = 0;
    TCCR0B = 0;
}
