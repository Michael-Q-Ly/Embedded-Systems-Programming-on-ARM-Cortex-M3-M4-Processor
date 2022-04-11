/*
 * main.c
 *
 *  Created on: Feb 10, 2022
 *  Author: Michael Ly
 */

#include "main.h"
#include "led.h"
#include <stdio.h>
#include <stdint.h>

uint8_t current_task = 1 ;										// Task1 is running
uint32_t g_tick_count = 0 ;
uint32_t const const_v_1 = 100 ;
uint32_t const const_v_2 = 100 ;
uint8_t const const_v_3 = 100 ;

/* Function Prototypes */
// extern void initialise_monitor_handles( void ) ;							// Debugger
void idle_task( void ) ;
void task1_handler( void ) ;                                                    			// This is task1
void task2_handler( void ) ;                                                    			// This is task2
void task3_handler( void ) ;                                                    			// This is task3
void task4_handler( void ) ;                                                    			// This is task4

__attribute__( (naked) ) void init_scheduler_stack( uint32_t sched_top_of_stack ) ;
void init_tasks_stack( void ) ;
void init_systick_timer( uint32_t tick_hz ) ;
void enable_processor_faults( void ) ;
__attribute__( (naked) ) void switch_sp_to_psp( void ) ;
uint32_t get_psp_value( void ) ;
void update_next_task( void ) ;
__attribute__( (naked) ) void PendSV_Handler( void ) ;							// This is our scheduler--what we use to do context switching
void SysTick_Handler( void ) ;

void HardFault_Handler( void ) ;
void MemManage_Handler( void ) ;
void BusFault_Handler( void ) ;

void task_delay( uint32_t tick_count ) ;
void schedule( void ) ;
void update_global_tick_count( void ) ;
void unblock_tasks( void ) ;

typedef struct {
	uint32_t psp_value ;
	uint32_t block_count ;
	uint8_t current_state ;
	void ( *task_handler )( void ) ;
} TCB_t ;

TCB_t user_tasks[MAX_TASKS] ;

int main(void) {
	// initialise_monitor_handles() ;                                          			// Debugger

	enable_processor_faults() ;

	init_tasks_stack() ;

	// Initialize the scheduler stack (which is MSP)
	init_scheduler_stack( SCHED_STACK_START ) ;

	// Task stack initialization to store the dummy frames  
	init_tasks_stack() ;

	// Initialize all of the LEDs
	led_init_all() ;

	// Generate SysTick timer exception
	init_systick_timer( TICK_HZ ) ;
	
	// Switch from MSP to PSP
	switch_sp_to_psp() ;

	task1_handler() ;

	/* Loop forever */
	for(;;);
}

void idle_task( void ) {
	while ( 1 ) ;
}

void task1_handler( void ) {
	while ( 1 ) {
		printf( "This is task1\n" ) ;
		led_on( LED_GREEN ) ;
		task_delay( 1000 ) ;
		led_off( LED_GREEN ) ;
		task_delay( 1000 ) ;
	}
}

void task2_handler( void ) {
	while ( 1 ) {
		printf( "This is task2\n" ) ;
		led_on( LED_RED ) ;
		task_delay( 500 ) ;
		led_off( LED_RED ) ;
		task_delay( 500 ) ;
	}
}

void task3_handler( void ) {
	while ( 1 ) {
		printf( "This is task3\n" ) ;
		led_on( LED_GREEN ) ;
		task_delay( 250 ) ;
		led_off( LED_RED ) ;
		task_delay( 250 ) ;
	}
}

void task4_handler( void ) {
	while ( 1 ) {
		printf( "This is task4\n" ) ;
		led_on( LED_RED ) ;
		task_delay( 155 ) ;
		led_off( LED_RED ) ;
		task_delay( 155 ) ;
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
	*pSRVR &= ~ ( 0x00FFFFFFFF ) ;									// Only 24 bits are valid in this register
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
	// All tasks should start in the ready state
	user_tasks[0].current_state = TASK_READY_STATE ;
	user_tasks[1].current_state = TASK_READY_STATE ;
	user_tasks[2].current_state = TASK_READY_STATE ;
	user_tasks[3].current_state = TASK_READY_STATE ;
	user_tasks[4].current_state = TASK_READY_STATE ;

	user_tasks[0].psp_value = IDLE_STACK_START ;
	user_tasks[1].psp_value = T1_STACK_START ;
	user_tasks[2].psp_value = T2_STACK_START ;
	user_tasks[3].psp_value = T3_STACK_START ;
	user_tasks[4].psp_value = T4_STACK_START ;

	// User function pointers to run tasks; review function pointers if you do not understand this part
	user_tasks[0].task_handler = idle_task ;
	user_tasks[1].task_handler = task1_handler ;
	user_tasks[2].task_handler = task2_handler ;
	user_tasks[3].task_handler = task3_handler ;
	user_tasks[4].task_handler = task4_handler ;

	uint32_t *pPSP ;
	for ( int i = 0 ; i < MAX_TASKS ; i++ ) {
		pPSP = ( uint32_t* )( user_tasks[i].psp_value ) ;
		
		pPSP-- ;										// xPSR
		*pPSP = DUMMY_XPSR ;									// 0x01000000

		pPSP-- ;										// PC
		*pPSP = ( uint32_t )user_tasks[i].task_handler ;

		pPSP-- ;										// LR
		*pPSP = 0xFFFFFFFD ;

		// Registers R0 through R12:
		for ( int j = 0 ; j < 13 ; j++ ) {
			pPSP-- ;
			*pPSP = 0 ;
		}

		// Preserve the value of PSP once you have loaded the Stack Frame.
		user_tasks[i].psp_value = ( uint32_t)( pPSP ) ;
	}
}

void enable_processor_faults( void ) {
	uint32_t *pSHCSR = ( uint32_t* )( 0xE000ED24 ) ;						// System Handler Control & State Register
	*pSHCSR |= ( 1 << 16 ) ;	// Memory Manage
	*pSHCSR |= ( 1 << 17 ) ;	// Bus Fault
	*pSHCSR |= ( 1 << 18 ) ;	// Usage Fault
}

uint32_t get_psp_value( void ) {
	return user_tasks[current_task].psp_value ;
}

void save_psp_value( uint32_t current_psp_value ) {
	user_tasks[current_task].psp_value = current_psp_value ;
}

void update_next_task( void ) {
	int state = TASK_BLOCKED_STATE ;

	for ( int i = 0 ; i < MAX_TASKS ; i++ ) {
		current_task++ ;
		current_task %= MAX_TASKS ;								// Task 0 through MAX_TASKS-1
		state = user_tasks[current_task].current_state ;
		if (    ( state == TASK_READY_STATE ) && ( current_task != 0 )    ) {			// Ignore idle_task
			break ;
		}
	}

	// If all states are blocked (not ready), we know we are in idle_task
	if ( state != TASK_READY_STATE ) {
		current_task = 0 ;
	}
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

void schedule( void ) {
	// Pend the PendSV exception
	uint32_t *pICSR = ( uint32_t* )0xE000ED04 ;
	*pICSR |= ( 1 << 28 ) ;
}

void task_delay( uint32_t tick_count ) {
	/*
	 * To avoid a race condition for shared memory, disable then reenable interrupts
	 */
	// Disable all interrupts
	INTERRUPT_DISABLE() ;

	// Only block the user tasks
	if ( current_task ) {
		user_tasks[current_task].block_count = g_tick_count + tick_count ;
		user_tasks[current_task].current_state = TASK_BLOCKED_STATE ;
		schedule() ;
	}

	// Enable interrupts
	INTERRUPT_ENABLE() ;
}

__attribute__( (naked) ) void PendSV_Handler( void ) {
	/* Save the context of current task */

	// 1. Get current running task's PSP value
	__asm volatile ( "MRS R0, PSP" ) ;	
	// 2. Using that PSP value, store SF2 (R4 to R11)
	__asm volatile ( "STMDB R0!, {R4-R11}" ) ;							// Store multiple registers into memory; "!" allows R0 to get updated after each store
	__asm volatile ( "PUSH {LR}" ) ;
	// 3. Save the current value of PSP
	__asm volatile ( "BL save_psp_value" ) ;	

	/* Retrieve the context of the next task */
	
	// 1. Decide next task to run
	__asm volatile ( "BL update_next_task" ) ;		
	// 2. Get its past PSP value
	__asm volatile ( "BL get_psp_value" ) ;
	// 3. Using that PSP value, retrieve SF2 (R4 to R11)
	__asm volatile ( "LDMIA R0!, {R4-R11}" ) ;
	// 4. Update PSP and exit
	__asm volatile ( "MSR PSP, R0" ) ;
	__asm volatile ( "POP {LR}" ) ;
	__asm volatile ( "BX LR" ) ;

	/**
	 * Summary of function:
	 * Saving context of current task:
	 * We get the current running task's private stack pointer by storing special register
	 * PSP into general register R0.
	 * We then store multiple registers (R4 through R11) into R0, but we decrement and refresh R0 after each store.
	 * We then preserve LR with a PUSH instruction.
	 * Next, we branch and link to save_psp_value. Why? Because we must come back to SysTick Handler AFTER we exit the
	 * function we are calling.
	 *
	 * Retrieving the context of the next task:
	 * We do another branch and link with function update_next_task.
	 * We then get the next PSP value by doing another branch and link with get_psp_value, which returns
	 * psp_of_tasks[current_task].
	 * Next we load multiple registers (R4 through R11) and update/increment R0 after each store.
	 * We then store the general register R0 into PSP
	 * and then POP LR.
	 * Finally, we do a branch indirect LR so that we can return to main()
	 */
}

void update_global_tick_count( void ) {
	g_tick_count++ ;
}

void unblock_tasks( void ) {
	for ( int i = 1 ; i < MAX_TASKS ; i++ ) {
		if ( user_tasks[i].current_state != TASK_READY_STATE ) {
			if ( user_tasks[i].block_count == g_tick_count ) {
				user_tasks[i].current_state = TASK_READY_STATE ;
			} /* if ( user_tasks[i]. block_count == g_tick_count ) */
		} /* if ( user_tasks[i].current_state != TASK_READY_STATE ) */
	} /* END FOR */
}

void SysTick_Handler( void ) {
	uint32_t *pICSR = ( uint32_t* )0xE000ED04 ;
	update_global_tick_count() ;
	unblock_tasks() ;
	// Pend the PendSV exception
	*pICSR |= ( 1 << 28 ) ;
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
