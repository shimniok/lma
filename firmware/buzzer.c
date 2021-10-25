#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.h"

inline void initBuzzer()
{
    // Need 3.8kHz tone, 150kHz/2/3800
    // set freq with output compare register
    // Prescale /1: CS02=0 CS01=0 CS00=1
    TCNT0=0;
    OCR0A  = F_CPU/2/3800;

    // Set Buzzer pins as output
    DDRB |= _BV(BUZZA)|_BV(BUZZB);
#if DEBUG
    DDRB |= _BV(LED);
#endif
}

void beepOn()
{
    // Interrupt, no toggle gpio: COM0A1=0 (TCCR0A:7), COM0A0=0 (TCCR0A:6) for non-PWM (CTC) mode
    // CTC mode -- clear on timer compare: WGM02=0 WGM01=1 WGM00=0
    // TIMSK - OCIE0A (bit 4): Timer/Counter0 Output Compare Match A Interrupt Enable
    TCCR0A = _BV(WGM01); // CTC mode
    TCCR0B = _BV(CS00);  // no clock prescaling
    TIMSK = _BV(OCIE0A); // Timer/Counter0 Output Compare Match A Interrupt Enable
    //PORTB |= _BV(BUZZA); // turn on one pin (set up for toggling)
    #if DEBUG
      PORTB |= _BV(LED);
    #endif
}

void beepOff()
{
    // disable timer, clock
    TCCR0A = 0;
    TCCR0B = 0;
    PORTB &= ~(_BV(BUZZA)|_BV(BUZZB)); // turn off pins
    #if DEBUG
      PORTB &= ~_BV(LED);
    #endif
}

ISR(TIMER0_COMPA_vect) {
  static int toggle = 0;
  if (toggle) {
    PORTB &= _BV(BUZZA);
    PORTB |= _BV(BUZZB);
  } else {
    PORTB &= _BV(BUZZB);
    PORTB |= _BV(BUZZA);
  }
}
