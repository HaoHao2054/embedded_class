#include <stdint.h>

// GPJ2 寄存器地址
#define GPJ2CON  (*(volatile uint32_t *)0xE0200280)  // 配置寄存器
#define GPJ2DAT  (*(volatile uint32_t *)0xE0200284)  // 数据寄存器

// GPJ2CON 位域定义
#define GPJ2CON_INPUT  0x0  // 输入模式
#define GPJ2CON_OUTPUT 0x1  // 输出模式

// LED 引脚定义
#define LED0 (0x1 << 0)  // GPJ2_0
#define LED1 (0x1 << 1)  // GPJ2_1
#define LED2 (0x1 << 2)  // GPJ2_2
#define LED3 (0x1 << 3)  // GPJ2_3

// 初始化 GPIO
void gpio_init(void) {
    // 设置 GPJ2_0 到 GPJ2_3 为输出模式
    GPJ2CON &= ~(0xF << 0);  // 清除 GPJ2_0 配置
    GPJ2CON |= (GPJ2CON_OUTPUT << 0);  // 设置 GPJ2_0 为输出

    GPJ2CON &= ~(0xF << 4);  // 清除 GPJ2_1 配置
    GPJ2CON |= (GPJ2CON_OUTPUT << 4);  // 设置 GPJ2_1 为输出

    GPJ2CON &= ~(0xF << 8);  // 清除 GPJ2_2 配置
    GPJ2CON |= (GPJ2CON_OUTPUT << 8);  // 设置 GPJ2_2 为输出

    GPJ2CON &= ~(0xF << 12); // 清除 GPJ2_3 配置
    GPJ2CON |= (GPJ2CON_OUTPUT << 12); // 设置 GPJ2_3 为输出
}

// 点亮 LED
void led_on(uint32_t led) {
    GPJ2DAT |= led;  // 点亮指定 LED
}

// 熄灭 LED
void led_off(uint32_t led) {
    GPJ2DAT &= ~led; // 熄灭指定 LED
}

// 切换 LED 状态
void led_toggle(uint32_t led) {
    GPJ2DAT ^= led;  // 切换指定 LED 状态
}

// 简单的延时函数
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 1000; i++) {
        __asm__ volatile("nop");  // 空操作
    }
}

// 主程序
int main(void) {
    gpio_init();  // 初始化 GPIO

    while (1) {
        // 点亮所有 LED
        led_on(LED0 | LED1 | LED2 | LED3);
        delay_ms(500);  // 延时 500ms

        // 熄灭所有 LED
        led_off(LED0 | LED1 | LED2 | LED3);
        delay_ms(500);  // 延时 500ms

        // 逐个点亮 LED
        led_on(LED0);
        delay_ms(200);
        led_on(LED1);
        delay_ms(200);
        led_on(LED2);
        delay_ms(200);
        led_on(LED3);
        delay_ms(200);

        // 逐个熄灭 LED
        led_off(LED0);
        delay_ms(200);
        led_off(LED1);
        delay_ms(200);
        led_off(LED2);
        delay_ms(200);
        led_off(LED3);
        delay_ms(200);
    }

    return 0;
}