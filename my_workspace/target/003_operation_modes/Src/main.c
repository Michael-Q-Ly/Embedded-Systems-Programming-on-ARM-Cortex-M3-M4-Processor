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

#include <stdint.h>
#include <stdio.h>

/* Function Prototype(s) */
extern void initialise_monitor_handles(void) ;									// Debugger
void generate_interrupt(void) ;													// Executes in THREAD MODE of the processor
void RTC_WKUP_IRQHandler(void) ;

/* This function executes in the THREAD MODE + PRIV ACCESS LEVEL of the processor */
int main(void)
{
	initialise_monitor_handles() ;												// Debugger

	printf("In thread mode: before interrupt\n") ;

	generate_interrupt() ;

	printf("In thread mode: after interrupt\n") ;

    /* Loop forever */
	for(;;);
}

/* This function executes in the THREAD MODE of the processor */
void generate_interrupt(void) {
	uint32_t	*pSTIR	= (uint32_t*) 0xE000EF00 ;
	uint32_t	*pISER0	= (uint32_t*) 0xE000E100 ;

	// Enable IRQ3 interrupt
	*pISER0 |= (1 << 3) ;

	// Generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF) ;
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode: ISR\n") ;
}
