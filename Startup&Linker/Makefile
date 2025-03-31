CC=arm-non-eabi-gcc
MACH=cortex-m4
CFLAGS= -c -mcpu=$ (MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS= -nostdlib -T stm32_linker.ld


all:main.o startup_stm32.o final.elf

main.o:main.c
	$(CC) $ (CFLAGS) -o $@ $^

startup_stm32.o:startup_stm32.c
	$(CC) $ (CFLAGS) -o $@ $^

final.elf:main.o startup_stm32.o
	$(CC) $ (LDFLAGS) -o $@ $^
	
clean:
	rm -rf *.o *.elf