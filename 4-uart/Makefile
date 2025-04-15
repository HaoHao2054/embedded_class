# Toolchain definitions
CC := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc
LD := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-ld
OBJCOPY := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objcopy
OBJDUMP := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objdump

# Build target
uart.bin: start.o clock.o uart.o main.o
	$(LD) -Ttext 0xD0020010 -o uart.elf $^
	$(OBJCOPY) -O binary uart.elf $@
	$(OBJDUMP) -D uart.elf > uart.dis

# Compilation rules
%.o: %.c
	$(CC) -c $< -o $@ -nostdlib

%.o: %.S
	$(CC) -c $< -o $@ -nostdlib

# Clean rule
clean:
	rm -f *.o *.elf *.bin *.dis