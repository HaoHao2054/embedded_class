#include "s5pv210.h"

void irqHandler() {
    volatile unsigned char key_code = get_irqstatus() & 0x3; // VIC0's status
    clear_vectaddr();
    clear_int_pend();      /* 清pending位 */
    
    if(key_code == 1)     /* key1 */
        led1_on_off();
    else if(key_code == 2) /* key2 */
        led2_on_off();
    else {
        led1_on_off();
        led2_on_off();
    }
}


// 左流水灯函数（从LED1到LED4依次点亮）
void left_flow_led(void)
{
    // 初始状态：熄灭所有LED
    GPJ2DAT |= ALL_LEDS;
    
    while (1) {
        // LED1 -> LED2 -> LED3 -> LED4 依次点亮
        led_on(LED1);
        delay(0x50000);
        led_off(LED1);
        
        led_on(LED2);
        delay(0x50000);
        led_off(LED2);
        
        led_on(LED3);
        delay(0x50000);
        led_off(LED3);
        
        led_on(LED4);
        delay(0x50000);
        led_off(LED4);
    }
}

// 右流水灯函数（从LED4到LED1依次点亮）
void right_flow_led(void)
{
    // 初始状态：熄灭所有LED
    GPJ2DAT |= ALL_LEDS;
    
    while (1) {
        // LED4 -> LED3 -> LED2 -> LED1 依次点亮
        led_on(LED4);
        delay(0x50000);
        led_off(LED4);
        
        led_on(LED3);
        delay(0x50000);
        led_off(LED3);
        
        led_on(LED2);
        delay(0x50000);
        led_off(LED2);
        
        led_on(LED1);
        delay(0x50000);
        led_off(LED1);
    }
}

/**
 * @brief 主函数
 * 初始化硬件并进入主循环
 */
int main(void)
{
    /* 硬件初始化 */
    led_init();    // LED初始化
    init_key();    // 按键初始化
    init_int();    // 中断初始化
    
    while(1);
}