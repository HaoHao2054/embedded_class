# 工具链路径设置
TOOLCHAIN_PATH = /usr/local/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin
CC = $(TOOLCHAIN_PATH)/arm-none-linux-gnueabihf-gcc
LD = $(TOOLCHAIN_PATH)/arm-none-linux-gnueabihf-ld
OBJCOPY = $(TOOLCHAIN_PATH)/arm-none-linux-gnueabihf-objcopy
OBJDUMP = $(TOOLCHAIN_PATH)/arm-none-linux-gnueabihf-objdump

# 目标文件
TARGET = uart
OBJS = start.o uart.o

# 编译选项
CFLAGS = -nostdlib -I. -O2 -Wall
LDFLAGS = -Ttext 0xD0020010

all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -D $< > $(TARGET).dis

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.elf *.bin *.dis