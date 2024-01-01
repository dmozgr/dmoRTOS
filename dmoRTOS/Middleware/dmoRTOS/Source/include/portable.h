#ifndef PORTABLE_H
#define PORTABLE_H

void xPortSchedulerStart();
uint8_t xPortTaskCreate( void(*funcPt)(void), uint16_t period, uint8_t priority);
void xPortTaskYield();

#endif
