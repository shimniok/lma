#include "config.h"
#include "switch.h"

bool pressed=0;

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
  return pressed;
}

void switchReset(void)
{
  pressed = 0;
}

// ISR goes here
void ISRthing()
{
  // debounce??
  pressed = 1;
}
