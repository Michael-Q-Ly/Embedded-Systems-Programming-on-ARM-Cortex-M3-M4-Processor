/*
 * led.h
 *
 *  Created on: 28-Dec-2019
 *      Author: nieki
 */

#ifndef LED_H_
#define LED_H_
#define STM32F42xx
#include <stdint.h>

#define RCC_BASE_ADDR				0x40023800U

#ifdef STM32F42xx
/* Shift amounts */
#define LED_GREEN	13
#define LED_RED		14

#else
#define LED_GREEN  12
#define LED_ORANGE 13
#define LED_RED    14
#define LED_BLUE   15
#endif /* STM32F42xx */

#define DELAY_COUNT_1MS				1250U
#define DELAY_COUNT_1S				(1000U * DELAY_COUNT_1MS)
#define DELAY_COUNT_500MS			(500U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_250MS			(250U  * DELAY_COUNT_1MS)
#define DELAY_COUNT_125MS			(125U  * DELAY_COUNT_1MS)

extern void led_init_all( void ) ;
extern void led_on( uint8_t led_no ) ;
extern void led_off( uint8_t led_no ) ;
extern void delay( uint32_t count ) ;

#endif /* LED_H_ */
