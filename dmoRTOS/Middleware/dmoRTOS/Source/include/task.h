#ifndef TASK_H
#define TASK_H

#include <stdint.h>

//void xTaskCreate(void(*func)(void), uint8_t threadID);
void xTaskCreate(void(*funcPt)(void), uint16_t period, uint8_t priority);
void xTaskStartScheduler( void );
void xTaskYield( void );
void xTaskWait( void );

#endif
