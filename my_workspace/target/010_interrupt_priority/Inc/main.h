/*
 * main.h
 *
 *  Created on: Jul 23, 2021
 *      Author: Micha
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>

#if ( !defined(__SOFT_FP__) && defined(__ARM_FP) )
    #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Function Prototypes */
extern void initialise_monitor_handles(void) ;									// Debugger
void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority_value) ;



#endif /* MAIN_H_ */
