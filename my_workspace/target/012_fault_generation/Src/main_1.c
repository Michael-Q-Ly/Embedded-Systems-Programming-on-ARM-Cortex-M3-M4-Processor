/*
 * main_1.c
 *
 *  Created on: Feb 7, 2022
 *      Author: Micha
 */

/**
 * Exercise:
 * Write a program to enable all configurable fault exceptions, implement
 * the fault exception handlers and cuase the fault by the following methods:
 *
 * 1. Execute an undefined instruction
 * 2. Divide by zero.
 * 3. Try executing an instruction from the peripheral region
 * 4. Execute SVC inside the SVC handler
 * 5. Execute an SVC instruction inside the interrupt handler whose
 *    priority is the same or lesser than the SVC handler
 */


#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

extern void initialise_monitor_handles(void) ;								// Debugger
void HardFault_Handler( void ) ;
void MemManage_Handler( void ) ;
void BusFault_Handler( void ) ;
__attribute__( (naked) ) void UsageFault_Handler( void ) ;
void UsageFault_Handler_c( uint32_t *pBaseStackFrame ) ;
<<<<<<< HEAD
void fun_divide( int x, int y ) ;
=======
int fun_divide( int x, int y ) ;
>>>>>>> a7e49dd (Add Example 12 (two versions):)

int main( void ) {
	initialise_monitor_handles() ;									// Debugger

	// 1. Enable all configurable exceptions like usage fault, memory manage fault, and bus fault
	uint32_t *pSHCSR = ( uint32_t* )( 0xE000ED24 ) ;						// System Handler Control & State Register
	// Use the table to see which bits to set the usage fault, memory manage fault, and bus fault: bits 16, 17, & 18 for Cortex M4
	*pSHCSR |= ( 1 << 16 ) ;	// Memory Manage
	*pSHCSR |= ( 1 << 17 ) ;	// Bus Fault
	*pSHCSR |= ( 1 << 18 ) ;	// Usage Fault

	// 2. Implement the fault handlers (Done outside of main; see below)

	// 3. Enable divide-by-zero trap
	uint32_t *pCCR = ( uint32_t* )( 0xE000ED14 ) ;
	*pCCR |= ( 1 << 4 ) ;

	// 4. Attempt to divide by zero
	fun_divide( 10, 0 ) ;
	// 5. Analyze the fault

	/* Loop forever */
	for( ; ; ) ;
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

__attribute__ ( (naked) ) void UsageFault_Handler( void ) {						// Caller
	__asm ( "MRS r0, MSP" ) ;									// Contents of r0 become argument 1 for the callee
	__asm( "B UsageFault_Handler_c" ) ;

}

void UsageFault_Handler_c( uint32_t *pBaseStackFrame ) {						// Callee
	// 4. Analyze the fault
	uint32_t *pUFSR = ( uint32_t* )( 0xE000ED2A ) ;
	fprintf( stderr, "Exception: Usagefault\n" ) ;
	printf( "UFSR = %lx\n", ( *pUFSR ) & 0xFFFF ) ;							// We only use the first 16 bits, so clear the lower 16 bits with an & 0xFFFF
	printf( "pBaseStackFrame = %p\n", pBaseStackFrame ) ;
	printf( "Value of R0 = %lx\n", pBaseStackFrame[0] ) ;
	printf( "Value of R1 = %lx\n", pBaseStackFrame[1] ) ;
	printf( "Value of R2 = %lx\n", pBaseStackFrame[2] ) ;
	printf( "Value of R3 = %lx\n", pBaseStackFrame[3] ) ;
	printf( "Value of R12 = %lx\n", pBaseStackFrame[4] ) ;
	printf( "Value of LR = %lx\n", pBaseStackFrame[5] ) ;
	printf( "Value of PC = %lx\n", pBaseStackFrame[6] ) ;
	printf( "Value of xPSR = %lx\n", pBaseStackFrame[7] ) ;

	while( 1 ) ;
}

<<<<<<< HEAD
void fun_divide( int x, int y ) {
=======
int fun_divide( int x, int y ) {
>>>>>>> a7e49dd (Add Example 12 (two versions):)
	return ( x / y ) ;
}

