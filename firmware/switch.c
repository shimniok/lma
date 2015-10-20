#include "config.h"
#include "switch.h"

bool pressed = 0;

void initSwitch(void) 
{
  // initialize ISR
 
  // Set switch as input
  DDRB &= ~_BV(SWITCH);
  
  // Set pullup on switch
  //PORTB |= _BV(SWITCH);
}
/*
bool switchPressed(void)
{
  return (PINB & _BV(SWITCH) == 0) ? 0 : 1;
}
*/
void switchReset(void)
{
  pressed = 0;
}


ISR(PCINT0_vect)
{
  // debounce??
  pressed = 1;
}
