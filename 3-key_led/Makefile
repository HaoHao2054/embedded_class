# Makefile
CC = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc
LD = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-ld
OBJCOPY = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objcopy
OBJDUMP = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objdump

all: key.bin

start.o: start.S
	$(CC) -nostdlib -o start.o start.S -c

key.o: key.c
	$(CC) -nostdlib -o key.o key.c -c

key.elf: start.o key.o
	$(LD) -Ttext 0xd0020010 -o key.elf start.o key.o

key.bin: key.elf
	$(OBJCOPY) -O binary key.elf key.bin
	$(OBJDUMP) -D key.elf > key.dis

clean:
	rm -f *.o *.elf *.bin *.dis