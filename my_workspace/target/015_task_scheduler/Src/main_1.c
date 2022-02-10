/*
 * main_1.c
 *
 *  Created on: Feb 10, 2022
 *      Author: Michael Ly
 */


#include <stdio.h>
#include <stdint.h>

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

