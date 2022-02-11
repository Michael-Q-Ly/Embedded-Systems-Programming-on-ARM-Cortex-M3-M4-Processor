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

uint8_t current_task = 0 ;										// Task1 is running

/* Function Prototypes */
extern void initialise_monitor_handles( void ) ;								// Debugger
void task1_handler( void ) ;                                                    				// This is task1
void task2_handler( void ) ;                                                    				// This is task2
void task3_handler( void ) ;                                                    				// This is task3
void task4_handler( void ) ;                                                    				// This is task4

__attribute__( (naked) ) void init_scheduler_stack( uint32_t sched_top_of_stack ) ;
void init_tasks_stack( void ) ;
void init_systick_timer( uint32_t tick_hz ) ;
void enable_processor_faults( void ) ;
__attribute__( (naked) ) void switch_sp_to_psp( void ) ;
uint32_t get_psp_value( void ) ;
void SysTick_Handler( void ) ;											// This is our scheduler--what we use to do context switching

void HardFault_Handler( void ) ;
void MemManage_Handler( void ) ;
void BusFault_Handler( void ) ;

int main(void)
{
	initialise_monitor_handles() ;                                          				// Debugger

	enable_processor_faults() ;

	// Initialize the scheduler stack (which is MSP)
	init_scheduler_stack( SCHED_STACK_START ) ;
	
	// Array-filling; capturing addresses of different task handlers
	task_handlers[0] = ( uint32_t )( task1_handler ) ;
	task_handlers[1] = ( uint32_t )( task2_handler ) ;
	task_handlers[2] = ( uint32_t )( task3_handler ) ;
	task_handlers[3] = ( uint32_t )( task4_handler ) ;

	// Task stack initialization to store the dummy frames  
	init_tasks_stack() ;

	// Generate SysTick timer exception
	init_systick_timer( TICK_HZ ) ;
	
	// Switch from MSP to PSP
	switch_sp_to_psp() ;

	task1_handler() ;

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

void enable_processor_faults( void ) {
	uint32_t *pSHCSR = ( uint32_t* )( 0xE000ED24 ) ;						// System Handler Control & State Register
	*pSHCSR |= ( 1 << 16 ) ;	// Memory Manage
	*pSHCSR |= ( 1 << 17 ) ;	// Bus Fault
	*pSHCSR |= ( 1 << 18 ) ;	// Usage Fault
}

uint32_t get_psp_value( void ) {
	return psp_of_tasks[current_task] ;
}

__attribute__( (naked) ) void switch_sp_to_psp( void ) {
	// 1. Initialize the PSP with TASK1 stack start

	// Get the value of PSP of current_task
	/**
	 * Note that we are still using MSP as our SP because we have not yet executed the switch of sp to psp!
	 *
	 * Also, since this function is called from main(), LR is holding some value that connects to main(). We call
	 * get_psp_value() in the function, though, so LR gets corrupted.
	 * Thus, we have to PUSH (save) LR because we want it later. We then pop it back.
	 */
	__asm volatile ( "PUSH {LR} " ) ;								// Preserve LR, which connects back to main()
	__asm volatile ( "BL get_psp_value" ) ;								// Branch and link to come back to the function; branch only takes you to the function
	__asm volatile ( "MSR PSP, R0" ) ;								// Initialize PSP
	__asm volatile ( "POP {LR} " ) ;								// Pops back LR value

	// 2. Change SP to PSP using CONTROL register
	// The CONTROL register is a special register, thus the function must be a naked function
	__asm volatile ( "MOV R0, #0x02" ) ;								// Refer to M4 User Manual: Switch to PSP
	__asm volatile ( "MSR CONTROL, R0" ) ;
	__asm volatile ( "BX LR" ) ;									// LR will be copied into PC, which takes us back to main()
}

void SysTick_Handler( void ) {

}

// 2. Implement the fault handlers
void HardFault_Handler( void ) {
	fprintf( stderr, "Exception: Hardfault\n" ) ;
	while( 1 ) ;
}

void MemManage_Handler( void ) {
	fprintf( stderr, "Exception: MemManage\n" ) ;
	while( 1 ) ;
}

void BusFault_Handler( void ) {
	fprintf( stderr, "Exception: Busfault\n" ) ;
	while( 1 ) ;
}

