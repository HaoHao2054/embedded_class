#include <stdint.h>

// 寄存器地址定义
#define GPJ2CON (*(volatile uint32_t *)0xE0200280)
#define GPJ2DAT (*(volatile uint32_t *)0xE0200284)
#define GPH0CON (*(volatile uint32_t *)0xE0200C00)
#define GPH0DAT (*(volatile uint32_t *)0xE0200C04)

// 模式选择宏（取消注释启用高级模式）
// #define ADVANCED_MODE

// 硬件定义
#define LED0     (0x1 << 3)
#define LED1     (0x1 << 2)
#define LED2     (0x1 << 1)
#define LED3     (0x1 << 0)
#define ALL_LEDS (LED0 | LED1 | LED2 | LED3)

#define SW5_PIN (1 << 3) // GPH0_3
#define SW6_PIN (1 << 4) // GPH0_4

// 初始化函数
void gpio_init(void)
{
    // 配置LED引脚为输出
    GPJ2CON = (GPJ2CON & ~0xFFFF) | 0x1111; // GPJ2_0~3设为输出

    // 配置按键引脚为输入（GPH0_3/4）
    GPH0CON &= ~((0xF << 12) | (0xF << 16)); // 清除配置位
}

// 带消抖的按键检测
uint8_t key_scan(void)
{
    static uint16_t key_history;
    static uint8_t debounce_cnt;
    const uint8_t DEBOUNCE_THRESHOLD = 20; // 消抖阈值（约20ms）
    uint8_t key_current = (~GPH0DAT) & (SW5_PIN | SW6_PIN);

    // 状态变化检测
    if (key_current != (key_history & 0x03))
    {
        debounce_cnt = DEBOUNCE_THRESHOLD;
        key_history = (key_history << 8) | key_current;
        return 0;
    }

    // 消抖计数
    if (debounce_cnt)
    {
        if (--debounce_cnt == 0)
        {
            return key_current;
        }
    }
    return 0;
}

// LED控制函数组
void led_toggle(uint32_t leds)
{
    GPJ2DAT ^= leds;
}
void led_on(uint32_t leds)
{
    GPJ2DAT |= leds;
}
void led_off(uint32_t leds)
{
    GPJ2DAT &= ~leds;
}

// 延时函数（约1ms）
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 1000; i++)
        __asm__("nop");
}

#ifdef ADVANCED_MODE
// 高级模式逻辑
void advanced_mode_handler(uint8_t key)
{
    if (key & SW5_PIN)
    { // SW5按下
        for (uint8_t i = 0; i < 5; i++)
        { // 闪烁5次
            led_on(ALL_LEDS);
            delay_ms(200);
            led_off(ALL_LEDS);
            delay_ms(200);
        }
        led_on(ALL_LEDS); // 全亮
    }
    if (key & SW6_PIN)
    {                      // SW6按下
        led_off(ALL_LEDS); // 全灭
    }
}
#else
// 基础模式逻辑
static uint8_t led_state; // LED状态缓存
void basic_mode_handler(uint8_t key)
{
    if (key & SW5_PIN)
        led_toggle(LED2); // 切换LED2
    if (key & SW6_PIN)
        led_toggle(LED3); // 切换LED3
}
#endif

int main(void)
{
    gpio_init();
    led_off(ALL_LEDS);

    while (1)
    {
        uint8_t key = key_scan();
        if (key)
        {
#ifdef ADVANCED_MODE
            advanced_mode_handler(key);
#else
            //basic_mode_handler(key);
            led_on(ALL_LEDS);
            delay_ms(200);
            led_off(ALL_LEDS);
            delay_ms(200);
#endif
        }
        delay_ms(1); // 降低CPU占用
    }
    return 0;
}