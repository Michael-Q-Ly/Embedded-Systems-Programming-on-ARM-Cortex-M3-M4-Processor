# File Name		: Makefile
# Name			: Michael Ly
# Date			: Apr 8, 2022

PROG = scheduler
CC = arm-none-eabi-gcc
MACH = cortex-m4
OBJS = main.o led.o stm32_startup.o
INCDIR = ../Inc
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -O0 -Wall -I$(INCDIR)
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=final.map -I../Inc

.PHONY: all
all: $(OBJS) final.elf

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o: led.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: load
load:
	openocd -f board/stm32f4discovery.cfg

.PHONY: clean
clean:
	rm -rf *.o *.elf *.exe
