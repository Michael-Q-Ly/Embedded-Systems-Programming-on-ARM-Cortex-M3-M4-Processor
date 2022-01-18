/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/*
 * Exercise 1:	Load 2 values from memory, add them, and store the result
 *				back to the memory using inline assembly statements.
 * Exercise 2:	Move the content of CONTROL register to 'C' variable
 *				"control_reg"
 * Exercise 3:	Copy the content of 'C' variable var1 to var2
 * Exercise 4:	Copy the contents of a pointer into another variable
 */

#include <stdint.h>

/* Function Prototype(s) */
extern void initialise_monitor_handles(void) ;									// Debugger

int main(void)
{
	initialise_monitor_handles() ;												// Debugger

#if 0
	__asm volatile ("LDR R1, =#0x20001000") ;									// Load register R1 with the address 0x20001000
	__asm volatile ("LDR R2, =#0x20001004") ;									// Load register R2 with the address 0x20001004
	__asm volatile ("LDR R0, [R1]") ;											// Load register R0 with the contents of R1 ; Load into R0 the memory location stored in R1 (Read / Memory Load)
	__asm volatile ("LDR R1, [R2]") ;											// Load register R1 with the contents of R2 (Read)
	__asm volatile ("ADD R0, R0, R1") ;											// Load register R0 with the sum of R0 and R1
	__asm volatile ("STR R0, [R2]") ;											// Store the result into the memory location of R2
#endif

	int val ;
	val = 50 ;
	__asm volatile ("MOV R0, %0" : : "r"(val)  : ) ;							// Constraint 'r' tells the compiler to use a general register.
#if 0	
	__asm volatile ("MOV R0, %0" : : "i"(0x50) : ) ;							// Constraint 'i' tells the compiler to use an immediate.
	__asm volatile ("MOV R0, %0" : : "r"(0x50) : ) ;
#endif

	int control_reg ;
	// Read CONTROL register to control_reg
	__asm volatile ("MRS %0, CONTROL" : "=r"(control_reg) ) ;

	/* Exercise 3 */
	int var1 ;
	int var2 ;
	var1 = 10 ;
	__asm ("MOV %0, %1" : "=r" (var2) : "r" (var1)) ;

	/* Exercise 4 */
	// This is essentially a memory read. p1 reads the contents of pointer *p2
	int p1 ;
	int *p2 ;
	p2 = (int*) 0x20000008 ;
	__asm volatile ("LDR %0, [%1]" : "=r" (p1): "r"(p2)) ;						// p1 = *p2

    /* Loop forever */
	for(;;);
}
