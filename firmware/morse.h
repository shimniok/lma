#ifndef __MORSE_H
#define __MORSE_H

// times in ms
#define DOT       60
#define DASH      DOT*3
#define SPACE     DOT    // intra character space
#define WORDSPACE DOT*7  // intra word space

#define space() _delay_ms(DOT*2)

#define OK "--. -.-"
#define SOS "... --- ..."
#define S "..."
#define W ".--"

void dit();
void dah();
void message(char *m);
void number(uint8_t n);

#endif
