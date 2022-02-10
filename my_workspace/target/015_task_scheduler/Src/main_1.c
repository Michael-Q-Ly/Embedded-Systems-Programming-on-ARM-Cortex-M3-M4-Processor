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

/* Function Prototypes */
extern void initialise_monitor_handles( void ) ;								// Debugger
void task1_handler( void ) ;                                                    				// This is task1
void task2_handler( void ) ;                                                    				// This is task2
void task3_handler( void ) ;                                                    				// This is task3
void task4_handler( void ) ;                                                    				// This is task4

int main(void)
{
	initialise_monitor_handles() ;                                          				// Debugger

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

