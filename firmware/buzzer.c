#include "config.h"
#include "clock.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buzzer.h"

uint16_t tim0_prescale[8] = {
  0,
  [TIM0_PRESCALE_1]1,
  [TIM0_PRESCALE_8]8,
  [TIM0_PRESCALE_64]64,
  [TIM0_PRESCALE_256]256UL,
  [TIM0_PRESCALE_1024]1024UL
};

uint16_t get_tim0_prescale()
{
  return tim0_prescale[TIM0_PRESCALE];
}

void beepOn()
{
  TCNT0 = 0;
  // Interrupt, no toggle gpio: COM0A1=0 (TCCR0A:7), COM0A0=0 (TCCR0A:6) for non-PWM (CTC) mode
  // CTC mode -- clear on timer compare: WGM02=0 WGM01=1 WGM00=0
  // TIMSK - OCIE0A (bit 4): Timer/Counter0 Output Compare Match A Interrupt Enable
  TCCR0A = _BV(WGM01); // CTC mode
  TCCR0B = TIM0_PRESCALE;
  OCR0A = (F_CLKDIV*F_CPU/BUZZER_FREQ)/2/get_cpu_prescale()/get_tim0_prescale();
  // Set Buzzer pins as output
  DDRB |= _BV(BUZZA)|_BV(BUZZB);

  #if DEBUG
    PORTB |= _BV(LED);
  #endif

  TIMSK |= _BV(OCIE0A); // Timer/Counter0 Output Compare Match A Interrupt Enable
}

void beepOff()
{
    // disable timer, clock
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK &= ~_BV(OCIE0A); // disable interrupt
    DDRB |= ~(_BV(BUZZA)|_BV(BUZZB));  // set buzzer pins as input
    PORTB &= ~(_BV(BUZZA)|_BV(BUZZB)); // turn off pins
    #if DEBUG
      PORTB &= ~_BV(LED);
    #endif
}

ISR(TIM0_COMPA_vect) {
  static int toggle = 0;
  if (toggle == 0) {
    PORTB &= ~_BV(BUZZA);
    PORTB |= _BV(BUZZB);
    toggle = 1;
  } else {
    PORTB &= ~_BV(BUZZB);
    PORTB |= _BV(BUZZA);
    toggle = 0;
  }
}
