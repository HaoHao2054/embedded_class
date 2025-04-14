#include <stdint.h>

// 寄存器地址定义
#define GPJ2CON (*(volatile uint32_t *)0xE0200280)
#define GPJ2DAT (*(volatile uint32_t *)0xE0200284)
#define GPH0CON (*(volatile uint32_t *)0xE0200C00)
#define GPH0DAT (*(volatile uint32_t *)0xE0200C04)

// LED 引脚定义（修正电平逻辑）
#define LED1     (0x1 << 0) // GPJ2_0（低电平点亮）
#define LED2     (0x1 << 1) // GPJ2_1
#define LED3     (0x1 << 2) // GPJ2_2
#define LED4     (0x1 << 3) // GPJ2_3
#define ALL_LEDS (LED4 | LED1 | LED2 | LED3)

// 按键引脚定义（保持原样）
#define SW5_PIN (1 << 3) // GPH0_3
#define SW6_PIN (1 << 4) // GPH0_4

// 模式选择宏
// #define ADVANCED_MODE

void gpio_init(void)
{
    // 保持原有GPIO配置
    GPJ2CON = (GPJ2CON & ~0xFFFF) | 0x1111;  // GPJ2_0~3设为输出
    GPH0CON &= ~((0xF << 12) | (0xF << 16)); // 配置按键输入模式

    // 初始化时熄灭所有LED（输出高电平）
    GPJ2DAT |= ALL_LEDS;
}

// 修正LED控制函数（低电平有效）
void led_on(uint32_t led)
{
    GPJ2DAT &= ~led;
} // 输出低电平
void led_off(uint32_t led)
{
    GPJ2DAT |= led;
} // 输出高电平
void led_toggle(uint32_t led)
{
    GPJ2DAT ^= led;
} // 保持异或逻辑

// 按键检测函数（保持原样）
uint8_t key_scan(void)
{
    return (~GPH0DAT) & (SW5_PIN | SW6_PIN); // 低电平有效
}

// // 延时函数保持不变
// void delay_ms(uint32_t ms) {
//     for(uint32_t i=0; i<ms*1000; i++) __asm__("nop");
// }

// 简单的延时函数
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 1000; i++)
    {
        __asm__ volatile("nop"); // 空操作
    }
}


// 修正后的高级模式逻辑
void led_blink_pattern(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        led_on(ALL_LEDS); // 实际输出低电平
        delay_ms(200);
        led_off(ALL_LEDS); // 实际输出高电平
        delay_ms(200);
    }
    led_on(ALL_LEDS); // 最终保持点亮
}


int main(void)
{
    gpio_init();

    // 初始化时全灭（实际输出高电平）
    led_off(ALL_LEDS);

    while (1)
    {
        uint8_t key = key_scan();

#ifdef ADVANCED_MODE
        if (key & SW5_PIN)
        {
            led_blink_pattern();
        }
        if (key & SW6_PIN)
        {
            led_off(ALL_LEDS); // 全灭（输出高电平）
        }

        led_on(ALL_LEDS); // 全亮（输出低电平）
        delay_ms(500);
        led_off(ALL_LEDS); // 全灭（输出高电平）
#else
        // 基础模式：状态切换逻辑修正
        static uint8_t last_key;
        if (key != last_key)
        {
            if (key & SW5_PIN)
                //led_toggle(LED2); // LED2状态翻转
                led_blink_pattern();

            if (key & SW6_PIN)
                //led_toggle(LED3); // LED3状态翻转
                led_off(ALL_LEDS); // 全灭（输出高电平）
            last_key = key;
        }
#endif
    }
    return 0;
}