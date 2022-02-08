/*
 * main_1.c
 *
 *  Created on: Feb 7, 2022
 *      Author: Michael Ly
 */

/**
 * Exercise:    Write a program to execure SVC instruction from thread mode,
 *              implement the SVC handler to print theSVC number used. Also,
 *              increment the SVC number by 4 and return it to the thread mode code
 *              and print it.
 *
 *              Hints:
 *              1)  Write a main() function where you should execute theSVC
 *                  instruction with an argument. Let's say SVC #0x5
 *              2)  Implement the SVC handler.
 *              3)	In the SVC handler, extract the SVC number and print it using printf.
 *              4)	Increment the SVC number by 4 and return it to the thread mode
 *              Hints:
 *              a) Write a main() function where you should execute the SVC instruction with
 *                 an argument. Let's say #0x5
 *		b) Implement the SVC handler
 *		c) In the SVC handler, extract the SVC number and print it using printf
 *		d) Increment the SVC number by 4 and return it to the Thread mod
 */


#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Function Prototypes */
extern void initialise_monitor_handles(void) ;									// Debugger
// 1. Create a function that operates in Thread mode using MSP
__attribute__ ( (naked) ) void SVC_Handler( void ) ;											// Look into startup file for this
void SVC_Handler_c(uint32_t *pBaseOfStackFrame) ;

int main(void) {
	initialise_monitor_handles() ;                                          	// Debugger
	
	// From ARM M4 manual 3.12.10 SVC Example: SVC #0x32 ; supervisor call (SVCall handler can extract
	// the immediate value by locating it through the stacked PC
	
	// 1. Create a function that operates in Thread mode using MSP.
	// 2. Create an SVC exception to create a stack frame in stack memory
	// 3. When SVC exception occurs, it runs in HANDLER mode; get the value of MSP
	// 3.1 Use in-line assembly to get the value of MSP and do a +6 to obtain pc register.
	// 4. Extract the opcode of the SVC instruction by subtracting 2 since
	//    SVC_Number = *( (next_ins_addr_after_svc) - 2 ).
	// 5. Return to Thread mode after adding 4 to the SVC number
	// 6. Print the value of the data returned

	// 2. Create an SVC exception to create a stack frame in stack memory
	__asm ( "SVC #0x5" ) ;								// Takes you to SVC_Handler assembly function

	printf( "Back in Thread Mode:\n" ) ;
	uint32_t svc_number_altered ;
	__asm volatile ( "MOV %0, R0" : "=r"( svc_number_altered ) : : ) ;		// Recall =r is 2 constraints. r is register operand, = is write-only constraint
	printf( "Altered SVC number in Thread Mode is: %ld\n", svc_number_altered ) ;

	/* Loop forever */
	for(;;);
}



__attribute__ ( (naked) ) void SVC_Handler( void ) {
	__asm ( "MRS r0, MSP" ) ;							// Store the MSP special reg into the R0 reg
	__asm ( "B SVC_Handler_c" ) ;							// and branch to the SVC Handler C function
}

void SVC_Handler_c(uint32_t *pBaseOfStackFrame) {
	printf( "In SVC Handler\n" ) ;

	// The SVC Handler function above uses R0 as the first argument of the new stack frame.
	// 3. Get the value of MSP and PC
	printf( "The value of R0 is:   %lx\n", pBaseOfStackFrame[0] ) ;
	printf( "The value of R1 is:   %lx\n", pBaseOfStackFrame[1] ) ;
	printf( "The value of R2 is:   %lx\n", pBaseOfStackFrame[2] ) ;
	printf( "The value of R3 is:   %lx\n", pBaseOfStackFrame[3] ) ;
	printf( "The value of R12 is:  %lx\n", pBaseOfStackFrame[4] ) ;
	printf( "The value of LR is:   %lx\n", pBaseOfStackFrame[5] ) ;
	printf( "The value of PC is:   %lx\n", pBaseOfStackFrame[6] ) ;
	printf( "The value of xPSR is: %lx\n", pBaseOfStackFrame[7] ) ;
	
	// 3.1 Add 6 to MSP to obtain PC
	uint8_t *pProgram_counter = ( uint8_t* )( pBaseOfStackFrame[6] ) ;		// Make these byte-addressable!
	pProgram_counter -= 2 ;

	// 4. Extract the opcode
	uint8_t svc_number ;
	svc_number = *pProgram_counter ;

	printf( "The SVC Number in Handler Mode is: %d\n", svc_number ) ;

	// 5. Return to Thread mode after adding 4 to the SVC number
	printf( "Adding 4 to the svc number and returning to Thread Mode:\n" ) ;
	svc_number += 4 ;
	pBaseOfStackFrame[0] = svc_number ;						// Store the new number in R0
}

