#ifndef __switch_h__
#define __switch_h__

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h> 
#include <avr/eeprom.h>

typedef uint8_t bool;

void initSwitch(void);
bool switchPressed(void);

#endif
