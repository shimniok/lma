#ifndef __BUZZER_H
#define __BUZZER_H

#include "config.h"

#define TIM0_PRESCALE_1    _BV(CS00)
#define TIM0_PRESCALE_8    _BV(CS01)
#define TIM0_PRESCALE_64   (_BV(CS01)|_BV(CS00))
#define TIM0_PRESCALE_256  _BV(CS02)
#define TIM0_PRESCALE_1024 (_BV(CS02)|_BV(CS01))

// Need 3.8kHz tone; set freq w/ TIM0_PRESCALE and COMP_VALUE
// COMP_VALUE = floor(F_CPU / F_PRESCALE / 2 / 3800 / TIM0_PRESCALE)
// Use higher TIM0_PRESCALE if COMP_VALUE > 255
#define TIM0_PRESCALE TIM0_PRESCALE_64
#define BUZZER_FREQ 3800

void beepOn();
void beepOff();

#endif
