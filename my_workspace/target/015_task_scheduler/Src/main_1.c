/*
 * main_1.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Michael Ly
 */

#include <stdio.h>
#include <stdint.h>
#include "main_1.h"

uint32_t psp_of_tasks[MAX_TASKS] = {	T1_STACK_START
					,	T2_STACK_START
					,	T3_STACK_START
					,	T4_STACK_START
} ;

uint32_t task_handlers[MAX_TASKS] ;

/* Function Prototypes */
extern void initialise_monitor_handles( void ) ;								// Debugger
void task1_handler( void ) ;                                                    				// This is task1
void task2_handler( void ) ;                                                    				// This is task2
void task3_handler( void ) ;                                                    				// This is task3
void task4_handler( void ) ;                                                    				// This is task4

__attribute__( (naked) ) void init_scheduler_stack( uint32_t sched_top_of_stack ) ;
void init_tasks_stack( void ) ;
void init_systick_timer( uint32_t tick_hz ) ;
void SysTick_Handler( void ) ;											// This is our scheduler--what we use to do context switching

int main(void)
{
	initialise_monitor_handles() ;                                          				// Debugger

	enable_processor_faults() ;

	// Initialize the scheduler stack (which is MSP)
	init_scheduler_stack( SCHED_STACK_START ) ;
	
	// Array-filling; capturing addresses of different task handlers
	task_handlers[0] = ( uint32_t) ( task1_handler ) ;
	task_handlers[1] = ( uint32_t) ( task2_handler ) ;
	task_handlers[2] = ( uint32_t) ( task3_handler ) ;
	task_handlers[3] = ( uint32_t) ( task4_handler ) ;

	// Task stack initialization to store the dummy frames  
	init_tasks_stack() ;

	// Generate SysTick timer exception
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

__attribute__( (naked) ) void init_scheduler_stack( uint32_t sched_top_of_stack ) {
//	__asm volatile ( "MSR MSP, R0" ) ;
	__asm volatile( "MSR MSP, %0" : : "r" ( sched_top_of_stack ) : ) ;				// The GCC inline with C variable equivalent of above
	__asm volatile ( "BX LR" ) ;									// BX copies the value of LR into PC, where LR contains the return address
}

void init_tasks_stack( void ) {

	uint32_t *pPSP ;
	for (int i = 0 ; i < MAX_TASKS ; i++ ) {
		pPSP = ( uint32_t* )( psp_of_tasks[i] ) ;
		
		pPSP-- ;										// xPSR
		*pPSP = DUMMY_XPSR ;									// 0x01000000

		pPSP-- ;										// PC
		*pPSP = task_handlers[i] ;

		pPSP-- ;										// LR
		*pPSP = 0xFFFFFFFD ;

		// Registers R0 through R12:
		for ( int j = 0 ; j < 13 ; j++ ) {
			pPSP-- ;
			*pPSP = 0 ;
		}

		// Preserve the value of PSP once you have loaded the Stack Frame.
		psp_of_tasks[i] = ( uint32_t)( pPSP ) ;
	}
}

void SysTick_Handler( void ) {

}

