#ifndef _CONFIG_H
#define _CONFIG_H

#define DEBUG		1		// if you want debug LED output, uncomment this

#define BUZZA   PB1     // ATtiny25 Pin 6
#define BUZZB		PB2     // ATtiny25 Pin 7
#define SWITCH  PB4     // ATtiny25 Pin 3
#define LED     PB0     // ATtiny25 Pin 5
#define ADC     PB3     // ATtiny25 Pin 7
#define ADC_EN  SWITCH

#define PERIOD		  10		// Delay period between signal (seconds) -- initial period
#define PERIOD_LOW  60		// If battery voltage is very low, use long period

#endif
