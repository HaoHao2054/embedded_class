/*
SW5控制LED2，SW6控制LED3

SW5对应EINT3，对应GPH0_3
SW6对应EINT4，对应GPH0_4
*/

#include <stdint.h>

// GPH寄存器地址 按键
#define GPH0CON (*(volatile uint32_t *)0xE0200C00)
#define GPH0DAT (*(volatile uint32_t *)0xE0200C04)

// GPJ2 寄存器地址
#define GPJ2CON  (*(volatile uint32_t *)0xE0200280)  // 配置寄存器
#define GPJ2DAT  (*(volatile uint32_t *)0xE0200284)  // 数据寄存器

// GPJ2CON 位域定义
#define GPJ2CON_INPUT  0x0  // 输入模式
#define GPJ2CON_OUTPUT 0x1  // 输出模式

// LED 引脚定义
#define LED3 (0x1 << 0)  // GPJ2_0
#define LED2 (0x1 << 1)  // GPJ2_1
#define LED1 (0x1 << 2)  // GPJ2_2
#define LED0 (0x1 << 3)  // GPJ2_3

/* 配置GPH0_3和GPH0_4为输入模式 */
void key_init(void)
{
    // 清除GPH0_3的配置位[15:12]，设置为输入模式（0000）
    GPH0CON &= ~(0xF << 12);

    // 清除GPH0_4的配置位[19:16]，设置为输入模式（0000）
    GPH0CON &= ~(0xF << 16);
}

/* 检测按键状态 */
int check_keys(void)
{
    // 读取GPH0DAT寄存器状态
    uint32_t data = GPH0DAT;

    // SW5检测（GPH0_3，bit3），低电平有效
    int sw5_pressed = !(data & (1 << 3));

    // SW6检测（GPH0_4，bit4），低电平有效
    int sw6_pressed = !(data & (1 << 4));

    return (sw5_pressed | (sw6_pressed << 1));
}


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

/* 使用示例 */
int main()
{
    key_init(); // 初始化按键
    gpio_init();  // 初始化 GPIO

    while (1)
    {
        int key_state = check_keys();

        if (key_state & 0x01)
        {
            // SW5被按下时的处理（控制LED）
        }
        if (key_state & 0x02)
        {
            // SW6被按下时的处理（控制LED）
        }

        // 建议添加去抖动延时（10-20ms）
    }
    return 0;
}