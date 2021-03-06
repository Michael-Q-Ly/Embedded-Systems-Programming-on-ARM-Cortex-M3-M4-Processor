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

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Function Prototypes */
extern void initialise_monitor_handles(void) ;									// Debugger

#define USART3_IRQN0    39

int main(void)
{
    initialise_monitor_handles() ;                                          	// Debugger

    // 1. Manually pend the pending bit for the IRQ number in  NVIC
    uint32_t *pISPR1 = (uint32_t*) (0xE000E200 + 0x0004) ;
    *pISPR1 |= (1 << (USART3_IRQN0 % 32) ) ;                                    // Do %32 since the register is 32b wide ; 39%32 = 7th bit position

    // 2. Enable the USART3 IRQ number in NVIC
    uint32_t *pISER1 = (uint32_t*) (0xE000E100 + 0x0004) ;
    *pISER1 |= (1 << (USART3_IRQN0 % 32)) ;

    /* Loop forever */
	while (1) {
        ;																		// NOP
    }
}

// USART3 ISR found from startup file
void USART3_IRQHandler(void) {
    printf("In USART3 ISR\n") ;
}
