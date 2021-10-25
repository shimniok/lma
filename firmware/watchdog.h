#ifndef __watchdog_h
#define __watchdog_h

void initWatchdog(uint16_t warn, uint16_t sos);
void enableWatchdog();
void disableWatchdog();

#endif
