#include "task.h"
#include "portable.h"

void xTaskCreate(void(*funcPt)(void), uint16_t period)
{
	xPortTaskCreate(funcPt, uint16_t period);
}
void xTaskStartScheduler( void )
{
	xPortSchedulerStart();
}

void xTaskYield(  )
{
	xPortTaskYield(  );
}

