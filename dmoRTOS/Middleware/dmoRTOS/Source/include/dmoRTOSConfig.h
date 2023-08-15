#ifndef DMORTOS_CONFIG_H
#define DMORTOS_CONFIG_H

#define configCPU_CLOCK_HZ                       	( 16000000UL )
#define configSYSTICK_CLOCK_HZ 						configCPU_CLOCK_HZ
#define configTICK_RATE_HZ                      	( 1000 )
#define configQUANTA								( 10 )

#define vPortSVCHandler                             SVC_Handler
#define xPortPendSVHandler                          PendSV_Handler
#define xPortSysTickHandler                         SysTick_Handler

#endif
