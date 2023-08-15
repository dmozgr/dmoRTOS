#include "ST/STM32F4xx/Include/stm32f407xx.h"

#include "dmoRTOS.h"
#include "portable.h"
#include "tcb_config.h"

#include <stdint.h>
#include <stdlib.h>


#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI3_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )
/* ...then bits in the registers. */
#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT		( 1UL << 16UL )
#define portNVIC_SYSTICK_CLK_BIT			( 1UL << 2UL )
#define portNVIC_PENDSVCLEAR_BIT 			( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT		( 1UL << 25UL )

void vPortTaskStackInit(int i, int32_t programCounter);
void vPortSchedulerLaunch();
uint8_t vPortSetupTimerInterrupt();

void vPortTaskStackInit(int i, int32_t programCounter)
{
	/* set tcb's stack pointer*/
	tcbs[i].stackPt = &TCB_STACK[i][STACK_SIZE - 16];
	/*PSR to 1, to operate in thumb mode*/
	TCB_STACK[i][STACK_SIZE - 1] = ( 1U << 24); /* PSR */

	TCB_STACK[i][STACK_SIZE - 3] = 0x12345603;
	TCB_STACK[i][STACK_SIZE - 4] = 0x12345604;
	TCB_STACK[i][STACK_SIZE - 5] = 0x12345605;
	TCB_STACK[i][STACK_SIZE - 6] = 0x12345606;
	TCB_STACK[i][STACK_SIZE - 7] = 0x12345607;
	TCB_STACK[i][STACK_SIZE - 8] = 0x12345608;

	TCB_STACK[i][STACK_SIZE - 9] = 0x12345609;
	TCB_STACK[i][STACK_SIZE - 10] = 0x12345610;
	TCB_STACK[i][STACK_SIZE - 11] = 0x12345611;
	TCB_STACK[i][STACK_SIZE - 12] = 0x12345612;
	TCB_STACK[i][STACK_SIZE - 13] = 0x12345613;
	TCB_STACK[i][STACK_SIZE - 14] = 0x12345614;
	TCB_STACK[i][STACK_SIZE - 15] = 0x12345615;
	TCB_STACK[i][STACK_SIZE - 16] = 0x12345616;
}

uint8_t xPortTaskCreate( void(*funcPt)(void), uint8_t threadID)
{
	__disable_irq();

//	tcbs[0].nextPt = &tcbs[1];
//	tcbs[1].nextPt = &tcbs[2];
//	tcbs[2].nextPt = &tcbs[0];
//
//	vPortTaskStackInit(0, 0x11);
//	TCB_STACK[0][STACK_SIZE - 2] = (int32_t)(funcPt1);
//	vPortTaskStackInit(1, 0x11);
//	TCB_STACK[1][STACK_SIZE - 2] = (int32_t)(funcPt2);
//
//	vPortTaskStackInit(2, 0x11);
//	TCB_STACK[2][STACK_SIZE - 2] = (int32_t)(funcPt3);
//    currentPt = &tcbs[0];

	tcbs[threadID].nextPt = &tcbs[(threadID + 1) % NUM_OF_THREADS];
	TCB_STACK[threadID][STACK_SIZE - 2] = (uint32_t)funcPt;
	vPortTaskStackInit(threadID, (uint32_t)(funcPt));
	currentPt = &tcbs[0];
	__enable_irq();
	return 1;
}

uint8_t vPortSetupTimerInterrupt()
{
	/* Stop and clear the SysTick. */
	portNVIC_SYSTICK_CTRL_REG = 0UL;
	portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

	/* Configure SysTick to interrupt at the requested rate. */
	portNVIC_SYSTICK_LOAD_REG = configQUANTA * ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;

	portNVIC_SYSPRI3_REG = (portNVIC_SYSPRI3_REG&0x00FFFFFF)|0xE0000000; // priority 7
	portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );

	__enable_irq();
	return 1;
}

void vPortSchedulerLaunch()
{
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

	/*return from exception*/
	__asm("BX	LR");
}

__attribute__((naked)) void xPortSysTickHandler( void )
{
	__asm("CPSID	I");

	/* save r4,r5,r6,r7,r8,r9,r10,r11*/

	__asm("PUSH {R4-R11}");

	/* Load address of currentPt into r0*/

	__asm("LDR R0,=currentPt");

	__asm("LDR R1,[R0]");

	__asm("STR SP,[R1]");

	/*load r1 from a location 4 bytes above address r1, i.e r1 = currentPt -> nextPt*/

	__asm("LDR R1,[R1,#4]");

	__asm("STR R1,[R0]");

	__asm("LDR SP,[R1]");

	/* restore r4,r5,r6,r7,r8,r9,r10,r11*/

	__asm("POP {R4-R11}");

	//enable global interrupts

	__asm("CPSIE	I");

	/* return from exception*/
	__asm("BX 	LR");
}

void xPortSchedulerStart()
{
	vPortSetupTimerInterrupt();
	vPortSchedulerLaunch();
}

