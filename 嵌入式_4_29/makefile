int.bin:start.o main.o
	arm-linux-ld -Ttext 0xD0020010 -o int.elf $^
	arm-linux-objcopy -O binary int.elf $@
	arm-linux-objdump -D int.elf > int.dis

%.o:%.c
	arm-linux-gcc -c $< -o $@ -nostdlib
%.o:%.S
	arm-linux-gcc -c $< -o $@ -nostdlib

clean:
	rm *.o *.elf *.bin *.dis