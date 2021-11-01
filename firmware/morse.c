#include "config.h"
#include "clock.h"
#include <util/delay.h>
#include "morse.h"
#include "buzzer.h"


void dit()
{
	beepOn();
	wait_ms(DOT);
	beepOff();
	wait_ms(DOT); // intra-character space
}

void dah()
{
	beepOn();
	wait_ms(DASH);
	beepOff();
	wait_ms(DOT); // intra-character space
}

inline void space()
{
	wait_ms(DOT*2);
}

void message(char *m) {
	char *s;
	for (s = m; *s != 0; s++) {
		switch (*s) {
		case '.':
			dit();
			break;
		case '-':
			dah();
			break;
		case ' ':
			space();
			break;
		}
	}
}

// each beep represents 5 minutes; 3 beeps = 15 min, 5 beeps = 25 min
void fives(int n) 
{
	if (n) {
		do {
			dah();
		} while ((n -= 5) > 0);
		space();
	}
}

// not actually morse numbers..
void number(uint8_t n)
{
	while (n >= 10) {
		dah();
		n -= 10;
	}
	if (n > 0) {
		dit();
	}
}
