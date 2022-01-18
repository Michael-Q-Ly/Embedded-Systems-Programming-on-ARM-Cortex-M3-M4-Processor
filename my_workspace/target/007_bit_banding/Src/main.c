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

// Exercise:
// Modify the content of the memory location 0x2000_0200 using the usual method 
// and bit-banding method. Analyze the difference.
//      First store the value 0xff into the memory location 0x2000_0200
//      Make the 7th bit position of the value to 0.
// General Formula:
//      Alias_Address = alias_base + [32 * (bit_band_memory_addr - bit_band_base)] + (bit * 4)
// Refer to the Cortex M4 Generic User Guide (Page 25)

#include <stdint.h>
#include <stdio.h>

#define MEMORY_LOCATION     0x20000200
#define LOCATION_MASK       1 << 7
#define ALIAS_BASE          0x22000000
#define BIT_BAND_BASE       0x20000000
#define BIT                 7

/* Function Prototype(s) */
extern void initialise_monitor_handles(void) ;									        // Debugger

int main(void) {
	initialise_monitor_handles() ;												        // Debugger

    uint8_t *ptr = (uint8_t*)(MEMORY_LOCATION) ;
    
    *ptr = 0xFF ;                                                                       // Dereference pointer to store

    // printf("0x%X is stored in location %p\n", *ptr, ptr) ;                           // Sets *ptr =0x00 for some reason

    // Normal Method

    // printf("Setting the 7th bit position to 0 using the normal method:\n") ;
    *ptr &= ~(LOCATION_MASK);
    // printf("0x%X is stored in location %p now\n", *ptr, ptr) ;

    // Reset ptr to 0xFF
    *ptr = 0xFF ;

    // Bit-Band Method
    uint8_t *alias_addr = (uint8_t*) ( ALIAS_BASE + ( 32 * (MEMORY_LOCATION - BIT_BAND_BASE) ) + (BIT * 4) ) ;
    // printf("Setting the 7th bit position to 0 using the bit-band method:\n") ;
    // Clearing 7th bit of address 0x20000200
    *alias_addr = 0 ;


    /* Loop forever */
	for(;;);
}
