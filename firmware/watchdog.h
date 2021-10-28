#ifndef __watchdog_h
#define __watchdog_h

extern uint16_t seconds;

void initWatchdog(uint16_t warn, uint16_t sos);
void enableWatchdog();
void disableWatchdog();

#endif
