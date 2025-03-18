# Makefile
CC = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc
LD = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-ld
OBJCOPY = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objcopy
OBJDUMP = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objdump

all: led.bin

start.o: start.S
	$(CC) -nostdlib -o start.o start.S -c

led.o: led.c
	$(CC) -nostdlib -o led.o led.c -c

led.elf: start.o led.o
	$(LD) -Ttext 0xd0020010 -o led.elf start.o led.o

led.bin: led.elf
	$(OBJCOPY) -O binary led.elf led.bin
	$(OBJDUMP) -D led.elf > led.dis

clean:
	rm -f *.o *.elf *.bin *.dis