/*
 * semphr.h
 *
 *  Created on: Dec 30, 2023
 *      Author: ozgur.delimehmetoglu
 */

#ifndef INCLUDE_SEMPHR_H_
#define INCLUDE_SEMPHR_H_

typedef struct semaphore_t semaphore_t;

struct semaphore_t
{
	int count;
};

semaphore_t xSemaphoreCreate(int initCount);
void vSemaphoreTake(semaphore_t *semphr);
void vSemaphoreGive(semaphore_t *semphr);

#endif /* INCLUDE_SEMPHR_H_ */
