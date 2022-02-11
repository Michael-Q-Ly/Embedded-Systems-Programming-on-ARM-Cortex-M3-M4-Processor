/*
 * main_1.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Michael Ly
 */

#include <stdio.h>
#include <stdint.h>

/* Some stack memory calculations */
#define SIZE_TASK_STACK			1024U
#define SIZE_SCHEDULER_STACK		1024U

#define SRAM_START			0x20000000U
#define SIZE_SRAM			( ( 128 ) * ( 1024 ) )
#define SRAM_END			( ( SRAM_START ) + ( SIZE_SRAM ) )

#define T1_STACK_START			SRAM_END
#define T2_STACK_START			( ( SRAM_END ) - ( 1 * SIZE_TASK_STACK ) )
#define T3_STACK_START			( ( SRAM_END ) - ( 2 * SIZE_TASK_STACK ) )
#define T4_STACK_START			( ( SRAM_END ) - ( 3 * SIZE_TASK_STACK ) )
#define SCHED_STACK_START		( ( SRAM_END ) - ( 4 * SIZE_TASK_STACK ) )

#define TICK_HZ				1000U

#define HSI_CLOCK			16000000U
#define SYSTICK_TIM_CLK			HSI_CLOCK

/* Function Prototypes */
extern void initialise_monitor_handles( void ) ;								// Debugger
void task1_handler( void ) ;                                                    				// This is task1
void task2_handler( void ) ;                                                    				// This is task2
void task3_handler( void ) ;                                                    				// This is task3
void task4_handler( void ) ;                                                    				// This is task4

void init_systick_timer( uint32_t tick_hz ) ;
void SysTick_Handler( void ) ;											// This is our scheduler--what we use to do context switching

int main(void)
{
	initialise_monitor_handles() ;                                          				// Debugger

	init_systick_timer( TICK_HZ ) ;

	/* Loop forever */
		for(;;);
}


void task1_handler( void ) {
	while ( 1 ) {
		printf( "This is task1\n" ) ;
	}
}

void task2_handler( void ) {
	while ( 1 ) {
		printf( "This is task2\n" ) ;

	}
}

void task3_handler( void ) {
	while ( 1 ) {
		printf( "This is task3\n" ) ;

	}
}

void task4_handler( void ) {
	while ( 1 ) {
		printf( "This is task4\n" ) ;

	}
}

void init_systick_timer( uint32_t tick_hz ) {
	/**
	 * Correct initialization sequence for systick timer is as follows (M4 guide page 252)
	 * 1. Program Reload Value Count
	 * 2. Clear current value of SVR
	 * 3. Program Control and Status register
	 */
	// Program the reload value
	uint32_t *pSRVR = ( uint32_t* )( 0xE000E014 ) ;							// Address of SysTick Reload Value Register
	uint32_t count_value = ( SYSTICK_TIM_CLK / tick_hz ) - 1 ;


	// Clear the value of SVR
	*pSRVR &= ~ ( 0x00FFFFFFFF) ;									// Only 24 bits are valid in this register
	// Load the value into SVR
	*pSRVR |= count_value ;

	// Modify the Control and Status Register settings
	uint32_t *pSCSR = ( uint32_t* )( 0xE000E010 ) ;							// Address of SysTick Control and Status Regiser
	*pSCSR |= ( 1 << 1 ) ;										// Enables SysTick exception request
	*pSCSR |= ( 1 << 2 ) ;										// Indicates the clock source as the processor clock

	// Enable the SysTick
	*pSCSR |= ( 1 << 0 ) ;										// Enables the counter
}

void SysTick_Handler( void ) {

}

