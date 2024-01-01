/*
 * portmacro.h
 *
 *  Created on: Dec 30, 2023
 *      Author: ozgur.delimehmetoglu
 */

#ifndef PORTABLE_GCC_ARM_CM4F_PORTMACRO_H_
#define PORTABLE_GCC_ARM_CM4F_PORTMACRO_H_


#define portNVIC_INTCTRL					( * ( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI3_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )

#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT		( 1UL << 16UL )
#define portNVIC_SYSTICK_CLK_BIT			( 1UL << 2UL )
#define portNVIC_PENDSVCLEAR_BIT 			( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT		( 1UL << 25UL )

#define MS_TO_TICKS(ms) ((ms) * (configSYSTICK_CLOCK_HZ ) / (configTICK_RATE_HZ ) - 1UL)

#endif /* PORTABLE_GCC_ARM_CM4F_PORTMACRO_H_ */
