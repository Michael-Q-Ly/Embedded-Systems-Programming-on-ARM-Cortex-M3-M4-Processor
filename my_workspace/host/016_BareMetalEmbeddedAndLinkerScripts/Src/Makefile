# File Name		: Makefile
# Name			: Michael Ly
# Date			: Apr 8, 2022

PROG = scheduler
CC = arm-none-eabi-gcc
MACH = cortex-m4
# OBJS = main.o led.o stm32_startup.o syscalls.o
OBJS = main.o led.o stm32_startup.o 
INCDIR = ../Inc
CFLAGS = -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft \
	 -std=gnu11 -O0 -Wall \
	 -I$(INCDIR)
# LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=final.map -I../Inc
# LDFLAGS = -mcpu=$(MACH) -mthumb -mfloat-abi=soft \
	  --specs=nano.specs -T \
	  stm32_ls.ld -Wl,-Map=final.map \
	  -I../Inc
LDFLAGS_SH = -mcpu=$(MACH) -mthumb -mfloat-abi=soft \
	  --specs=rdimon.specs -T \
	  stm32_ls.ld -Wl,-Map=final.map \
	  -I../Inc

.PHONY: all
# all: $(OBJS) final.elf
all: $(OBJS)

.PHONY: semi
semi: final_sh.elf

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o: led.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o: stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

final_sh.elf: $(OBJS)
	$(CC) $(LDFLAGS_SH) -o $@ $^

.PHONY: load
load:
	openocd -f board/stm32f4discovery.cfg

.PHONY: clean
clean:
	rm -rf *.o *.elf *.exe
