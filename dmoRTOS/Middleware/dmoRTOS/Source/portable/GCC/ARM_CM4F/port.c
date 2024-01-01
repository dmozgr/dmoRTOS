#include "ST/STM32F4xx/Include/stm32f407xx.h"

#include "dmoRTOS.h"
#include "portable.h"
#include "tcb_config.h"
#include "portmacro.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static taskcontrolblock_t *idleTask;
static void xPortCreateIdleHookTask(void(*funcPt)(void));
void xPortIdleHook(void);

void vPortTaskStackInit(taskcontrolblock_t *task ,int32_t programCounter);
void vPortSchedulerLaunch();
uint8_t vPortSetupTimerInterrupt();

void vPortTaskStackInit(taskcontrolblock_t *task ,int32_t programCounter)
{
	task->stack[STACK_SIZE - 2] = programCounter;
	task->tcb->stackPt = &task->stack[STACK_SIZE - 16];
	task->stack[STACK_SIZE - 1] = ( 1U << 24 ); /* PSR */

	task->stack[STACK_SIZE - 3] = 0x12345603;
	task->stack[STACK_SIZE - 4] = 0x12345604;
	task->stack[STACK_SIZE - 5] = 0x12345605;
	task->stack[STACK_SIZE - 6] = 0x12345606;
	task->stack[STACK_SIZE - 7] = 0x12345607;
	task->stack[STACK_SIZE - 8] = 0x12345608;
	task->stack[STACK_SIZE - 9] = 0x12345609;
	task->stack[STACK_SIZE - 10] = 0x12345610;
	task->stack[STACK_SIZE - 11] = 0x12345611;
	task->stack[STACK_SIZE - 12] = 0x12345612;
	task->stack[STACK_SIZE - 13] = 0x12345613;
	task->stack[STACK_SIZE - 14] = 0x12345614;
	task->stack[STACK_SIZE - 15] = 0x12345615;
	task->stack[STACK_SIZE - 16] = 0x12345616;
}

//void BOŞTABEKLEMEFONKSIYONU

uint8_t xPortTaskCreate( void(*funcPt)(void), uint16_t period)
{
	__disable_irq();
	taskcontrolblock_t* newTask = (taskcontrolblock_t*)malloc(sizeof(taskcontrolblock_t));

	if(taskList == NULL)
	{
		tcb_t *newTcb = (tcb_t*)malloc(sizeof(tcb_t));
		newTask->tcb = newTcb;
		vPortTaskStackInit(newTask, (uint32_t)(funcPt));
		currentTask = newTask;
		newTask->sleepTime = 0;
		newTask->state = BLOCKED;
		newTask->period = period;
		newTask->programCounter = funcPt;
		newTask->nextTask = newTask;
		taskList = newTask;
		xPortCreateIdleHookTask(&xPortIdleHook);
	}
	else
	{
		tcb_t *newTcb = (tcb_t*)malloc(sizeof(tcb_t));
		newTask->tcb = newTcb;
		vPortTaskStackInit(newTask, (uint32_t)(funcPt));
		newTask->state = BLOCKED;
		newTask->programCounter = funcPt;
		newTask->period = period;
		newTask->nextTask = taskList->nextTask;
		taskList->nextTask = newTask;
	}

	__enable_irq();
	return 1;
}

uint8_t vPortSetupTimerInterrupt()
{
	/* SysTick Timeri durdur ve temizle */
	portNVIC_SYSTICK_CTRL_REG = 0UL;
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

	/* SysTick timer'a istenilen zamanda interrupt uretebilmesi icin load degerini yukle */
	portNVIC_SYSTICK_LOAD_REG = configQUANTA * ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;

	portNVIC_SYSPRI3_REG = (portNVIC_SYSPRI3_REG&0x00FFFFFF)|0xE0000000; // priority 7
	portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );

	__enable_irq();
	return 1;
}

void vPortSchedulerLaunch()
{
	//taskcontrolblock_t* temp;
	//temp = taskList->nextTask;

	//do {
	//temp->tcb->nextPt = temp->nextTask->tcb;
	//temp = temp->nextTask;
	//} while (temp != taskList->nextTask);

	currentPt = currentTask->tcb;
	__asm("LDR R0,=currentPt");
	__asm("LDR R2,[r0]");
	__asm("LDR SP,[R2]");
	__asm("POP {R4-R11}");
	__asm("POP {R12}");
	__asm("POP {R0-R3}");
	__asm("ADD SP,SP,#4");
	__asm("POP {LR}");
	__asm("ADD SP,SP,#4");
	__asm("CPSIE 	I");

	/* Task fonksiyonuna dön */
	__asm("BX	LR");
}

void deneme()
{
	taskcontrolblock_t *temp = taskList;
//
//	do{
//		if( temp->state == READY )
//		{
//			currentTask = temp;
//			currentTask->state = RUNNING;
//			currentPt = currentTask->tcb;
//			goto exit;
//		}
//	}while( temp != taskList->nextTask);

	if( taskList != NULL )
	{
		do{
			if( temp->state != READY )
			{
				if( (temp->sleepTime <= temp->period))
				{
					temp->sleepTime += configQUANTA;
					temp->state = BLOCKED;
				}
				else{
					temp->sleepTime = 0;
					temp->state = READY;
				}
			}
			temp = temp->nextTask;

		} while(temp != taskList);
	}

	temp = taskList;

	do{
		if( temp->state == READY )
		{
			currentTask = temp;
			currentTask->state = RUNNING;
			currentPt = currentTask->tcb;
			goto exit;
		}
		temp = temp->nextTask;
	} while( temp != taskList);

	currentTask = idleTask;
	currentPt = idleTask->tcb;

	exit:
		__asm("nop");
//	if( currentTask->sleepTime < periodTick )
//	{
//		currentTask->sleepTime+=MS_TO_TICKS(configQUANTA);
//		currentTask = currentTask->nextTask;
//		xPortTaskYield();
//	}
//	currentPt = currentTask->tcb;
//	currentTask = currentTask->nextTask;
}

__attribute__((naked)) void xPortSysTickHandler( void )
{
	// Kesmeleri devre dışı bırak
	__asm("CPSID I");

	// Kaydet r4,r5,r6,r7,r8,r9,r10,r11
	__asm("PUSH {R4-R11}");

	// R0'a currentPt'yi yükle
	__asm("LDR R0,=currentPt");
	__asm("LDR R1,[R0]");
	__asm("STR SP,[R1]");

	// r1'i, currentPt'nin 4 byte üstündeki adres noktasından yükle, yani r1 = currentPt -> nextPt


	__asm("PUSH {R0,LR}");
	__asm("BL deneme");
	__asm("POP {R0,LR}");
	__asm("LDR R1,[R0]");
	//__asm("LDR R1,=tmp");
	//__asm("STR R1,[R0]");

	// SP'yi r1'den yükle
	__asm("LDR SP,[R1]");

	// r4,r5,r6,r7,r8,r9,r10,r11'i geri yükle
	__asm("POP {R4-R11}");

	// Global kesmeleri etkinleştir
	__asm("CPSIE I");

	// kesmeden dön
	__asm("BX LR");
}

void xPortSchedulerStart()
{
	vPortSetupTimerInterrupt();
	vPortSchedulerLaunch();
}

void xPortTaskYield(  )
{
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0x00;
	portNVIC_INTCTRL = 0x04000000;	// Trigger Systick
}

void xPortTaskWait()
{
	currentTask->state = BLOCKED;
	while(currentTask->state == BLOCKED);
}

uint32_t idleHookTick = 0;

void xPortIdleHook(void)
{
	while(1)
	{
		idleHookTick++;
	}
}

static void xPortCreateIdleHookTask(void(*funcPt)(void))
{
	idleTask = (taskcontrolblock_t*)malloc(sizeof(taskcontrolblock_t));

	tcb_t *newTcb = (tcb_t*)malloc(sizeof(tcb_t));
	idleTask->tcb = newTcb;
	vPortTaskStackInit(idleTask, (uint32_t)(funcPt));
	idleTask->sleepTime = 0;
	idleTask->state = IDLE;
	idleTask->period = 0;
}


