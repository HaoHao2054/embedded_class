#include <stdint.h>

// 寄存器地址定义
#define GPJ2CON  (*(volatile uint32_t *)0xE0200280)
#define GPJ2DAT  (*(volatile uint32_t *)0xE0200284)
#define GPH0CON  (*(volatile uint32_t *)0xE0200C00)
#define GPH0DAT  (*(volatile uint32_t *)0xE0200C04)

// LED 引脚定义（保持原有定义）
#define LED1 (0x1 << 0)  // GPJ2_0
#define LED2 (0x1 << 1)  // GPJ2_1
#define LED3 (0x1 << 2)  // GPJ2_2
#define LED4 (0x1 << 3)  // GPJ2_3
#define ALL_LEDS (LED4 | LED1 | LED2 | LED3)

// 按键引脚定义（根据GPH0DAT寄存器）
#define SW5_PIN (1 << 3)  // GPH0_3
#define SW6_PIN (1 << 4)  // GPH0_4

// 模式选择宏（取消注释启用高级模式）
// #define ADVANCED_MODE

void gpio_init(void) 
{
    // 保持原有LED初始化
    GPJ2CON = (GPJ2CON & ~0xFFFF) | 0x1111; // GPJ2_0~3设为输出
    
    // 配置按键引脚为输入（GPH0_3/4）
    GPH0CON &= ~((0xF << 12) | (0xF << 16)); // 清除配置位（设置为输入模式）
}

// 保持原有LED控制函数
void led_on(uint32_t led)  { GPJ2DAT |= led; }
void led_off(uint32_t led) { GPJ2DAT &= ~led; }
void led_toggle(uint32_t led) { GPJ2DAT ^= led; }

// 简化版按键检测（无消抖）
uint8_t key_scan(void) 
{
    // 根据GPH0DAT寄存器说明读取引脚状态
    return (~GPH0DAT) & (SW5_PIN | SW6_PIN); // 低电平有效
}

// 保持原有延时函数
void delay_ms(uint32_t ms) {
    for(uint32_t i=0; i<ms*1000; i++) __asm__("nop");
}

#ifdef ADVANCED_MODE
// 高级模式闪烁函数
void led_blink_pattern(void)
{
    for(uint8_t i=0; i<5; i++) {
        led_on(ALL_LEDS);
        delay_ms(200);
        led_off(ALL_LEDS);
        delay_ms(200);
    }
    led_on(ALL_LEDS); // 最终全亮
}
#endif

int main(void) 
{
    gpio_init();
    
    led_on(ALL_LEDS);

    while(1) {
        uint8_t key = key_scan();
        
        #ifdef ADVANCED_MODE
        if(key & SW5_PIN) {
            led_blink_pattern();
        }
        if(key & SW6_PIN) {
            led_off(ALL_LEDS);
        }
        #else
        // 基础模式：直接切换状态
        static uint8_t last_key;
        if(key != last_key) {
            if(key & SW5_PIN) led_toggle(LED2);
            if(key & SW6_PIN) led_toggle(LED3);
            last_key = key;
        }
        #endif
        
        // 保持原有LED演示逻辑（可注释掉实际使用时）
        // led_on(ALL_LEDS);
        // delay_ms(500);
        // led_off(ALL_LEDS);
        // delay_ms(500);
    }
    return 0;
}