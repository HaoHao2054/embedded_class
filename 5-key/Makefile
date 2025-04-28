CROSS = arm-linux-gnueabi-
CFLAGS = -march=armv5te -marm -nostdlib -fno-builtin -I.  # 添加-marm选项
ASFLAGS = -march=armv5te -marm  # 专门针对汇编文件的选项

OBJ = start.o main.o

all: led.bin

led.bin: $(OBJ)
	$(CROSS)ld -Ttext 0x40000000 -o led.elf $^
	$(CROSS)objcopy -O binary led.elf led.bin

%.o: %.c
	$(CROSS)gcc $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CROSS)gcc $(ASFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin