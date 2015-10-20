#ifndef _CONFIG_H
#define _CONFIG_H

//#define DEBUG		1		// if you want debug LED output, uncomment this

#define F_CPU		125000 // 8MHz / 64

#define BUZZ		0		// PB0
#define SWITCH  2   // PB2

#define PERIOD		  10		// Delay period between signal (seconds) -- initial period
#define PERIOD_LOW  60		// If battery voltage is very low, use long period

#endif

