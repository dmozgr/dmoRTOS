#include "task.h"
#include "portable.h"

void xTaskCreate(void(*funcPt)(void), uint8_t threadID)
{
	xPortTaskCreate(funcPt, threadID);
}
void xTaskStartScheduler( void )
{
	xPortSchedulerStart();
}
