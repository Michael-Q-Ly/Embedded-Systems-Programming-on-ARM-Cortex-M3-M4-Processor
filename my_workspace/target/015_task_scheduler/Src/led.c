// led.c
// STM32F429ZIT DISC1 only has 2 LEDS: Green (PG13) and Red (PG14)

#include "main.h"
#include "led.h"
#define STM32F42xx

/*
 * This function initializes the on-board LEDs
 */
void led_init_all( void ) {

	uint32_t *pRccAhb1enr   = ( uint32_t* )( 0x40023830 ) ;
	uint32_t *pGpioGModeReg = ( uint32_t* )( 0x40021800 ) ;

#ifdef STM32F42xx
	// Set clk enable for GPIO G
	*pRccAhb1enr |= ( 1 << 6 ) ;                                // Enable GpioG
	// Configure the LEDs output type as outputs
	*pGpioGModeReg |= ( 1 << ( 2 * LED_GREEN ) ) ;
	*pGpioGModeReg |= ( 1 << ( 2 * LED_RED ) ) ;

	led_off( LED_GREEN ) ;
	led_off( LED_RED ) ;
#else
	*pGpioGModeReg |= ( 1 << ( 2 * LED_GREEN ) ) ;
	*pGpioGModeReg |= ( 1 << ( 2 * LED_ORANGE ) ) ;             // TODO: Configure registers
	*pGpioGModeReg |= ( 1 << ( 2 * LED_RED ) ) ;
	*pGpioGModeReg |= ( 1 << ( 2 * LED_BLUE ) ) ;

	led_off( LED_GREEN ) ;
	led_off( LED_ORANGE ) ;
	led_off( LED_RED ) ;
	led_off( LED_BLUE ) ;
#endif /* STM32F42xx */
}

// Software delay; wastes clock cycles
// TODO: Maybe use a hardware timer instead in the future
/*
 * This function creates a software delay for the LEDs */
void delay( uint32_t count ) {
	for( uint32_t i = 0 ; i < count ; i++ ) ;
}

/*
 * This function turns the LED on and accepts as an argument the led color's port number
 */
void led_on( uint8_t led_no ) {
	// Add the Output Data Register (ODR) offset to the base address of GPIO G clk
	uint32_t *pGpioGDataReg = ( uint32_t* )( 0x40021814 ) ;
	*pGpioGDataReg |= ( 1 << led_no ) ;

}

/*
 * This function turns the LED off and accepts as an argument the led color's port number
 */
void led_off( uint8_t led_no ) {
	uint32_t *pGpioGDataReg = ( uint32_t* )( 0x40021814 ) ;
	*pGpioGDataReg &= ~( 1 << led_no ) ;
}
