#ifndef _CONFIG_H
#define _CONFIG_H

//#define PWM		1		// if your buzzer requires pwm, uncomment this
//#define DEBUG		1		// if you want debug LED output, uncomment this

#define BUZZ		0		// PB0
#define LED			2		// PB3

#define PERIOD		10		// Delay period between signal (seconds) -- initial period
#define VOLT_LOW    35		// considered low battery voltage; need to conserve more energy; ADC: 255/1.1V; 3.5V->214
#define PERIOD_LOW  60		// If battery voltage is very low, use long period
#define WARN_TIME   5*60	// Delay before warning stars sounding (seconds) every PERIOD seconds
#define SOS_TIME	15*60	// Delay before warning stops and SOS starts sounding (seconds) every PERIOD seconds

#endif