#include "task.h"
#include "portable.h"

void xTaskCreate(void(*funcPt)(void), uint16_t period)
{
	xPortTaskCreate(funcPt, period);
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

