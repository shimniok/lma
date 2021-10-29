#ifndef _CONFIG_H
#define _CONFIG_H

#define DEBUG 1

#define BUZZA   PB1     // ATtiny25 Pin 6
#define BUZZB   PB2     // ATtiny25 Pin 7
#define SWITCH  PB4     // ATtiny25 Pin 3
#define LED     PB0     // ATtiny25 Pin 5
#define ADC     PB3     // ATtiny25 Pin 2
#define ADC_EN  PB0     // ATtiny25 Pin 5

#define WARN_PERIOD      10   // Delay period between signal (seconds) -- initial period
#define WARN_PERIOD_LOW  60   // If battery voltage is low, use longer period between beeps
#define BATT_PERIOD     600   // Time in seconds between checking battery voltage

#endif
