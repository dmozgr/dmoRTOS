/*
 * semphr.c
 *
 *  Created on: Dec 30, 2023
 *      Author: ozgur.delimehmetoglu
 */
#include "semphr.h"
#include "ST/STM32F4xx/Include/stm32f407xx.h"

semaphore_t xSemaphoreCreate(int initCount)
{
	semaphore_t semphr;
	semphr.count = initCount;
}

void vSemaphoreTake(semaphore_t *semphr)
{
	__disable_irq();
	while( semphr->count <= 0 )
	{
		__disable_irq();
		__enable_irq();
	}
	semphr->count -= 1;
	__enable_irq();
}

void vSemaphoreGive(semaphore_t *semphr)
{
	__disable_irq();
	semphr->count += 1;
	__enable_irq();
}

