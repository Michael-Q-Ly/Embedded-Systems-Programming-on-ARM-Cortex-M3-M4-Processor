/*
 * File Name    : stm32_startup.c
 * Name         : Michael Ly
 * Date         : Apr 9, 2022
 * 
 * Description  : This is the startup file for the STM32F429ZIT MCU
 *                      Please check the reference manual's NVIC table when designing
 *                      the startup file.
 */
#include <stdint.h>

#define SRAM_START      0X20000000U
#define SRAM_SIZE       ( 128 * 1024 )  // 128kB
#define SRAM_END        ( (SRAM_START) + (SRAM_SIZE) )

#define STACK_START     SRAM_END

void Reset_Handler( void ) ;

// Create an array to hold MSP and handler's addresses
uint32_t vectors[] = {
        STACK_START
        ,       ( uint32_t )&Reset_Handler
} ;

void Reset_Handler( void ) {

}
