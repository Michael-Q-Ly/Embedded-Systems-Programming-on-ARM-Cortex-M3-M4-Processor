/*
 * main_1.c
 *
 *  Created on: Feb 8, 2022
 *      Author: Michael Ly
 */

/*
 * Write a program to add, subtract, multiply, and divide 2 operands using
 * SVC Handler and return the result to the thread mode code and print the
 * result. Thread mode should pass 2 operands via the stack frame.
 * Use the SVC number to determine the operation.
 *
 *      Service Number      Operation
 *      36                  Addition
 *      37                  Subtraction
 *      38                  Multiplication
 *      39                  Division
 */

// 1. Create the four operation functions
// 2. Call each function and have them print what operation they will perform
// 3. Further define each function by calling the SVC handler
// 4. Inside the SVC handler, use a switch statement to decide what to return

#include <stdio.h>
#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

enum operation {
	add = 36 ,
	sub ,
	mul ,
	div
} ;


// Function Prototype(s)
extern void initialise_monitor_handles(void) ;							// Debugger
__attribute__ ( (naked) ) void SVC_Handler( void ) ;
void SVC_Handler_c( uint32_t *pBaseOfStackFrame ) ;
int32_t add_nums( int32_t x, int32_t y ) ;
int32_t sub_nums( int32_t x, int32_t y ) ;
int32_t mul_nums( int32_t x, int32_t y ) ;
int32_t div_nums( int32_t x, int32_t y ) ;

int main(void) {
	initialise_monitor_handles() ;                                          		// Debugger
#if 1
	int32_t num0 ;
	int32_t num1 ;

	num0 = 5 ;
	num1 = 15 ;

	int32_t res ;

	res = add_nums( num0, num1 ) ;
	printf( "%ld + %ld = %ld\n", num0, num1, res ) ;

	res = sub_nums( num0, num1 ) ;
	printf( "%ld - %ld = %ld\n", num0, num1, res ) ;

	res = mul_nums( num0, num1 ) ;
	printf( "%ld * %ld = %ld\n", num0, num1, res ) ;

	res = div_nums( num0, num1 ) ;
	printf( "%ld / %ld = %ld\n", num0, num1, res ) ;
#else
	int32_t res ;
	res = add_nums( 10, 15 ) ;
	printf( "Add result is: %ld\n", res ) ;

	res = sub_nums( 5, -15 ) ;
	printf( "Sub result is: %ld\n", res ) ;

	res = mul_nums( 2, -3 ) ;
	printf( "Mul result is: %ld\n", res ) ;

	res = div_nums( 60, 12 ) ;
	printf( "Div result is: %ld\n", res ) ;

#endif
	/* Loop forever */
	for(;;);
}


__attribute__ ( (naked) ) void SVC_Handler( void ) {
	// Move the contents of MSP into R0
	__asm( "MRS r0, MSP" ) ;
	// Branch to SVC_Handler_c and pass it R0 as arg1
	__asm( "B SVC_Handler_c" ) ;
}

void SVC_Handler_c( uint32_t *pBaseOfStackFrame ) {
	printf( "Inside SVC_Handler\n" ) ;

	// Declare two arguments to apply an operation on as well as a result variable
	int32_t arg0 ;
	int32_t arg1 ;
	int32_t res ;

	// Add 6 to MSP to get PC
	uint8_t *pReturn_addr = ( uint8_t* )( pBaseOfStackFrame[6] ) ;
	// Subtract 2 from return address to get SVC value
	pReturn_addr -= 2 ;
	// Set the SVC number
	uint8_t svc_number ;
	svc_number = *pReturn_addr ;
	printf( "SVC number is: %d\n", svc_number ) ;

	// Recall that R0 stores the first argument and R1 stores the second argument
	arg0 = pBaseOfStackFrame[0] ;
	arg1 = pBaseOfStackFrame[1] ;

	// Decide which operation to use based on SVC number
	switch ( svc_number ) {
		case add:	res = arg0 + arg1 ;	break ;
		case sub:	res = arg0 - arg1 ;	break ;
		case mul:	res = arg0 * arg1 ;	break ;
		case div:	res = arg0 / arg1 ;	break ;
		default:	fprintf( stderr, "Invalid SVC code\n" ) ;
	}

	// Store the result back to the stack frame at R0's position
	pBaseOfStackFrame[0] = res ;
}

int32_t add_nums( int32_t x, int32_t y ) {
//	printf( "In add_nums function\n" ) ;
	
	// Declare a variable to hold the result from the SVC call
	int32_t res ;
	__asm volatile ( "SVC #36") ;
	// Store the contents of R0 into the result
	__asm volatile ( "MOV %0, R0" : "=r"( res ) : : ) ;

	return res ;
}

int32_t sub_nums( int32_t x, int32_t y ) {
//	printf( "In sub_nums function\n" ) ;

	int32_t res ;
	__asm volatile ( "SVC #37") ;
	// Store the contents of R0 into the result
	__asm volatile ( "MOV %0, R0" : "=r"( res ) : : ) ;

	return res ;
}

int32_t mul_nums( int32_t x, int32_t y ) {
//	printf( "In mul_nums function\n" ) ;

	int32_t res ;
	__asm volatile ( "SVC #38") ;
	// Store the contents of R0 into the result
	__asm volatile ( "MOV %0, R0" : "=r"( res ) : : ) ;

	return res ;
}

int32_t div_nums( int32_t x, int32_t y ) {
//	printf( "In div_nums function\n" ) ;

	int32_t res ;
	__asm volatile ( "SVC #39") ;
	// Store the contents of R0 into the result
	__asm volatile ( "MOV %0, R0" : "=r"( res ) : : ) ;

	return res ;
}

