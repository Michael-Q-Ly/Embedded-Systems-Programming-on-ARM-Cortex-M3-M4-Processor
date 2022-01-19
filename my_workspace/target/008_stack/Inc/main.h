/*
 * main.h
 *
 *  Created on: Jul 16, 2021
 *      Author: Micha
 */

#ifndef MAIN_H_
#define MAIN_H_

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Definitions */
#define SRAM_START      (0x20000000U)
#define SRAM_SIZE       (192*1024)                                              // 192kb
#define SRAM_END        ( (SRAM_START) + (SRAM_SIZE) )
#define STACK_START     (SRAM_END)

#define STACK_MSP_START (STACK_START)
#define STACK_MSP_END   (STACK_MSP_START + 512)                                 // SP is 1kB, so divide into upper
#define STACK_PSP_START (STACK_MSP_END)                                         // and lower bits, each 512b wide

/* Function Prototype(s) */
extern void initialise_monitor_handles(void) ;									// Debugger
int fun_add(int a, int b, int c, int d) ;
__attribute__((naked)) void change_sp_to_psp(void) ;
void generate_exception(void) ;
void SVC_Handler(void) ;

#endif /* MAIN_H_ */
