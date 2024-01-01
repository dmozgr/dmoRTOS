#include "task.h"
#include "portable.h"

void xTaskCreate(void(*funcPt)(void), uint16_t period, uint8_t priority)
{
	xPortTaskCreate(funcPt, period, priority);
}
void xTaskStartScheduler( void )
{
	xPortSchedulerStart();
}

void xTaskYield(  )
{
	xPortTaskYield(  );
}

void xTaskWait( void )
{
	xPortTaskWait();
}

