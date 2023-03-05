#include "osKernel/osKernel.h"

uint32_t task0Profiller, task1Profiller, task2Profiller, periodicTask1Profiller,periodicTask2Profiller;

void task0(void)
{
	while(1)
	{
		task0Profiller++;
		osTaskYield();
	}
}

void task1(void)
{
	while(1)
	{
		task1Profiller++;
		osTaskYield();
	}
}

void task2(void)
{
	while(1)
	{
		task2Profiller++;
		osTaskYield();
	}
}

void periodicTask1(void)
{
		periodicTask1Profiller++;
}

void periodicTask2(void)
{
		periodicTask2Profiller++;
}
int main()
{	
	osKernelInit();
  osKernelPeriodicInit();

	
	osKernelTaskCreate(&task0);
	osKernelTaskCreate(&task1);
	osKernelTaskCreate(&task2);
	osKernelPeriodicTaskCreate(periodicTask1, 1000);
	osKernelPeriodicTaskCreate(periodicTask2, 1000);
	
	osKernelLaunch(10);
	
	while(1);
}