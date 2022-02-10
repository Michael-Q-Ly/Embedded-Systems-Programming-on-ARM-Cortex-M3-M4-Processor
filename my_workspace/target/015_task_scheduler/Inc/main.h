/*
 * main.h
 *
 *  Created on: Oct 19, 2021
 *      Author: Michael Ly
 */

#ifndef MAIN_H_
#define MAIN_H_

#define MAX_TASKS               4
#define SIZE_TASK_STACK         1024U
#define SIZE_SCHED_STACK        1024U

#define SRAM_START              0x20000000U
#define SIZE_SRAM               ( (128) * (1024) )
#define SRAM_END                ( (SRAM_START) + (SIZE_SRAM) )

#define T1_STACK_START          ( SRAM_END )
#define T2_STACK_START          ( (SRAM_END) - (1 * SIZE_TASK_STACK) )
#define T3_STACK_START          ( (SRAM_END) - (2 * SIZE_TASK_STACK) )
#define T4_STACK_START          ( (SRAM_END) - (3 * SIZE_TASK_STACK) )
#define SCHED_TASK_START        ( (SRAM_END) - (4 * SIZE_TASK_STACK) )

#define TICK_HZ                 1000U

#define HSI_CLOCK               16000000U
#define SYSTICK_TIM_CLK         HSI_CLOCK
#define SYST_RVR_ADDR           0xE000E014UL
#define SYST_CSR_ADDR           0xE000E010UL

#define DUMMY_XPSR                      0x01000000UL

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#endif /* MAIN_H_ */
