/*
   -schedular
	 -Thread Control Block
	 -Stack
*/

#include "osKernel.h"

#define NUM_OF_THREADS 	3
#define NUM_OF_PERIODIC_TASK	2
#define STACK_SIZE 		100
#define BUS_FREQ		16000000



static uint32_t MILLIS_PRESCALER;
void osSchedulerLaunch(void);

static uint8_t numOfAddedThread = 0;
static uint8_t numOfAddedPeriodicTask = 0;
static uint32_t periodTick = 0;

/*Systick priority register*/
#define SYSPRI3         (*((volatile uint32_t *)0xE000ED20))
#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))

struct periodicTask_t{
	uint32_t period;
	void (*task)();
};
typedef struct periodicTask_t periodicTask_t;

struct tcb_t{

  int32_t *stackPt;
	struct tcb_t *nextPt;
};
typedef struct tcb_t tcb_t;

static tcb_t tcbs[NUM_OF_THREADS];
static periodicTask_t periodicTasks[NUM_OF_PERIODIC_TASK];

tcb_t *currentPt;
static int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

void osKernelStackInit(int i){
   tcbs[i].stackPt =  &TCB_STACK[i][STACK_SIZE -16];//StackPointer
	 TCB_STACK[i][STACK_SIZE -1] =  0x01000000;

	TCB_STACK[i][STACK_SIZE-3] = 0x14141414;  //R14
	TCB_STACK[i][STACK_SIZE-4] = 0x12121212;  //R12
	TCB_STACK[i][STACK_SIZE-5] = 0x03030303;  //R3
	TCB_STACK[i][STACK_SIZE-6] = 0x02020202;  //R2
	TCB_STACK[i][STACK_SIZE-7] = 0x01010101;  //R1
	TCB_STACK[i][STACK_SIZE-8] = 0x00000000;  //R0
	  /*We have to save manually*/
	TCB_STACK[i][STACK_SIZE-9] = 0x11111111;  //R11
	TCB_STACK[i][STACK_SIZE-10] = 0x10101010; //R10
	TCB_STACK[i][STACK_SIZE-11] = 0x09090909; //R9
	TCB_STACK[i][STACK_SIZE-12] = 0x08080808; //R8
	TCB_STACK[i][STACK_SIZE-13] = 0x07070707; //R7
	TCB_STACK[i][STACK_SIZE-14] = 0x06060606; //R6
	TCB_STACK[i][STACK_SIZE-15] = 0x05050505; //R5
	TCB_STACK[i][STACK_SIZE-16] = 0x04040404; //R4
}


uint8_t osKernelTaskCreate(void(*task)(void))
{
	__disable_irq();
	tcbs[numOfAddedThread].nextPt = &tcbs[(numOfAddedThread + 1) % NUM_OF_THREADS];
	TCB_STACK[numOfAddedThread][STACK_SIZE - 2] = (uint32_t)task;
	osKernelStackInit(numOfAddedThread);

	numOfAddedThread = (numOfAddedThread + 1) % NUM_OF_THREADS;

	currentPt = &tcbs[0];

	__enable_irq();

	return 1;
}

void osKernelInit(void)
{
	MILLIS_PRESCALER =  (BUS_FREQ/1000);
}


void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL =0;
	SysTick->VAL  =0;
	SysTick->LOAD =  (quanta*MILLIS_PRESCALER)-1;

	SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7
	SysTick->CTRL =  0x00000007;

	RCC->APB1ENR |= 1<<0;
	TIM2->CR1 = 0;
	TIM2->PSC = 16-1;
	TIM2->ARR = 1000;
	TIM2->DIER |= 1<<0;
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= 1<<0;
	osSchedulerLaunch();
}

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~(1<<0);
	for( uint8_t i = 0; i < NUM_OF_PERIODIC_TASK; i++)
	{
		if( ( (periodTick) % periodicTasks[i].period) == 0 )
		{
			periodicTasks[i].task();
		}
	}
	periodTick++;
}

void osKernelPeriodicTaskCreate( void(*task)() ,uint32_t period)
{
	periodicTasks[numOfAddedPeriodicTask].period = period;
	periodicTasks[numOfAddedPeriodicTask].task = task;
	numOfAddedPeriodicTask++;
}

void osSchedulerRoundRobin(void)
{

	currentPt = currentPt->nextPt;
}

void osTaskYield(void)
{
	SysTick->VAL = 0;

	INTCTRL = 0x04000000;
}
