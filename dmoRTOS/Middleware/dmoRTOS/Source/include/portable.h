#ifndef PORTABLE_H
#define PORTABLE_H

//uint8_t xPortTaskCreate( void(*funcPt)(void), uint8_t threadID);
void xPortSchedulerStart();
uint8_t xPortTaskCreate( void(*funcPt)(void), uint8_t threadID);

#endif
