CC := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc
LD := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-ld
OBJCOPY := /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-objcopy

led.bin: start.o led.o
	$(LD) -Ttext 0xd0020010 -o led.elf $^
	$(OBJCOPY) -O binary led.elf led.bin

led.o: led.S
	$(CC) -nostdlib -c $< -o $@

start.o: start.S
	$(CC) -nostdlib -c $< -o $@

clean:
	rm -rf *.o *.elf *.bin