#ifndef _ADC_H
#define _ADC_H

#define initADC() ADMUX = _BV(REFS0) | _BV(MUX1) | _BV(MUX0)

uint8_t readVoltage();

#endif
