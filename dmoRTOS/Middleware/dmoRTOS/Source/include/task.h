#ifndef TASK_H
#define TASK_H

#include <stdint.h>

//void xTaskCreate(void(*func)(void), uint8_t threadID);
void xTaskCreate(void(*funcPt)(void), uint8_t threadID);
void xTaskStartScheduler( void );

#endif
