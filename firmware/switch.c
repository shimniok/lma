#include "config.h"
#include "switch.h"

#define pressed() ((PINB & _BV(SWITCH)) == 0);

void initSwitch(void)
{
  // initialize ISR

  // Set switch as input
  DDRB &= ~_BV(SWITCH);

  // Set pullup on switch
  PORTB |= _BV(SWITCH);
}
bool switchPressed(void)
{
  bool result = 0;

  result = pressed();
  _delay_ms(50); // debounce

  return result & pressed();
}
