#include "ST/STM32F4xx/Include/stm32f407xx.h"

#include "dmoRTOS.h"
#include "portable.h"
#include "tcb_config.h"
#include "portmacro.h"

#include <stdint.h>
#include <stdlib.h>

void vPortTaskStackInit(taskcontrolblock_t *task ,int32_t programCounter);
void vPortSchedulerLaunch();
uint8_t vPortSetupTimerInterrupt();

void vPortTaskStackInit(taskcontrolblock_t *task ,int32_t programCounter)
{
	task->stack[STACK_SIZE - 2] = programCounter;
	task->tcb.stackPt = &task->stack[STACK_SIZE - 16];
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

uint8_t xPortTaskCreate( void(*funcPt)(void), uint16_t period)
{
	__disable_irq();
	taskcontrolblock_t* newTask = (taskcontrolblock_t*)malloc(sizeof(taskcontrolblock_t));

	if(taskList == NULL)
	{
		vPortTaskStackInit(newTask, (uint32_t)(funcPt));
		newTask->state = READY;
		newTask->nextTask = newTask;
		taskList = newTask;
	}
	else
	{
		vPortTaskStackInit(newTask, (uint32_t)(funcPt));
		newTask->state = READY;
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
	taskcontrolblock_t* temp;
	temp = taskList->nextTask;

	do {
		temp->tcb.nextPt = &temp->nextTask->tcb;
		temp = temp->nextTask;
	} while (temp != taskList->nextTask);

	currentPt = &taskList->nextTask->tcb;
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
	__asm("LDR R1,[R1,#4]");
	__asm("STR R1,[R0]");

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
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0;
	portNVIC_INTCTRL = 0x04000000;	// Trigger Systick
}



