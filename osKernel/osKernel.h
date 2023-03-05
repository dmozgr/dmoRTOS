#ifndef __OS_KERNEL_H__
#define __OS_KERNEL_H__

#include "stdint.h"
#include "stm32f407xx.h"              // Device header

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
void osKernelPeriodicInit();
uint8_t osKernelTaskCreate(void(*task0)(void));
void osKernelPeriodicTaskCreate( void(*task)() ,uint32_t period);
void osTaskYield(void);

#endif
