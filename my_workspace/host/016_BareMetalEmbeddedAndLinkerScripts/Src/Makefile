# File Name		: Makefile
# Name			: Michael Ly
# Date			: Apr 8, 2022

PROG = scheduler
CC = arm-none-eabi-gcc
MACH = cortex-m4
INCDIR = ../Inc
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -O0 -Wall -I$(INCDIR)
OBJS = main.o led.o stm32_startup.o

.PHONY: all
all: $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o: led.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -rf *.o *.elf *.exe
