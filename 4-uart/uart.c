#include "s5pv210.h"

void clock_init()
{
    /* 1、设置PLL_LOCK寄存器（这里使用默认值） */

    /* 2、设置PLL_CON寄存器（使用芯片手册推荐的值） */
    APLLCON0 = (1 << 0) | (3 << 8) | (125 << 16) | (1 << 31); /* FOUTAPLL= 1000MHz */
    MPLLCON = (1 << 0) | (12 << 8) | (667 << 16) | (1 << 31); /* FOUTMPLL= 667MHz */
    EPLLCON0 = (2 << 0) | (3 << 8) | (48 << 16) | (1 << 31);  /* FOUTEPLL= 96MHz */
    VPLLCON = (3 << 0) | (6 << 8) | (108 << 16) | (1 << 31);  /* FOUTVPLL= 54MHz */

    /* 3、选择PLL为时钟输出 */
    /* MOUT_MSYS = SCLKAPLL = 1000MHz
    ​**​ MOUT_DSYS = SCLKMPLL = 667MHz
    ​**​ MOUT_PSYS = SCLKMPLL = 667MHz
    */
    CLK_SRC0 = (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12);

    /* 4、设置系统时钟分频值 */
    /* freq(ARMCLK) = MOUT_MSYS / (APLL_RATIO + 1) = 1000MHz / (0 +1)= 1000MHz
    ​**​ freq(HCLK_MSYS) = ARMCLK / (HCLK_MSYS_RATIO + 1) = 1000MHz /(4+ 1) = 200MHz
    ​**​ freq(PCLK_MSYS) = HCLK_MSYS / (PCLK_MSYS_RATIO + 1) = 200MHz /(1+ 1) = 100MHz
    ​**​ freq(HCLK_DSYS) = MOUT_DSYS / (HCLK_DSYS_RATIO + 1) = 667 / (3+1) = 166MHz
    ​**​ freq(PCLK_DSYS) = HCLK_DSYS / (PCLK_DSYS_RATIO + 1) = 166 / (1+1) = 83MHz
    ​**​ freq(HCLK_PSYS) = MOUT_PSYS / (HCLK_PSYS_RATIO + 1) = 667 / (4+1) = 133MHz
    ​**​ freq(PCLK_PSYS) = HCLK_PSYS / (PCLK_PSYS_RATIO + 1) = 133 / (1+1) = 66MHz
    */
    CLK_DIV0 = (0 << 0) | (4 << 8) | (1 << 12) | (3 << 16) | (1 << 20) | (4 << 24) | (1 << 28);

    /* 等待PLL稳定 */
    volatile unsigned int i;
    for (i = 0; i < 10000; i++);
}

// 延时函数
void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 5000; i++)
        __asm__("nop");
}

// LED初始化
void led_init(void)
{
    GPJ2CON = (GPJ2CON & ~0xFFFF) | 0x1111; // 配置GPJ2_0~3为输出
    GPJ2DAT |= ALL_LEDS;                    // 初始化为熄灭状态
}

// 跑马灯控制函数
void led_run_left(uint8_t cycles)
{
    for (uint8_t c = 0; c < cycles; c++)
    {
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED1;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED2;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED3;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED4;
        delay_ms(200);
    }
    GPJ2DAT |= ALL_LEDS;
}

void led_run_right(uint8_t cycles)
{
    for (uint8_t c = 0; c < cycles; c++)
    {
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED4;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED3;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED2;
        delay_ms(200);
        GPJ2DAT = (GPJ2DAT | ALL_LEDS) & ~LED1;
        delay_ms(200);
    }
    GPJ2DAT |= ALL_LEDS;
}

// UART初始化（基于前文代码优化）
void uart0_init(void)
{
    // GPA0配置
    GPA0CON = (GPA0CON & ~0xFF) | 0x22;

    // 波特率配置（PCLK=66MHz）
    UBRDIV0 = 34;
    UDIVSLOT0 = 0xDDDD;

    // 8N1配置
    ULCON0 = 0x3;
    UCON0 = 0x5;
}
// 发送一个字符
void uart0_putc(char c)
{
    // 等待发送缓冲区为空
    while (!(UTRSTAT0 & (1 << 1)));

    // 发送字符
    UTXH0 = c;
}

// 接收一个字符
char uart0_getc(void)
{
    // 等待接收缓冲区有数据
    while (!(UTRSTAT0 & (1 << 0)));

    // 返回接收到的字符
    return URXH0;
}

// 发送字符串
void uart0_puts(const char *str)
{
    while (*str)
        uart0_putc(*str++);
}

int main(void)
{
    // 初始化系统时钟
    clock_init();

    // 初始化UART0
    uart0_init();

    // 初始化LED
    led_init();

    uart0_puts("\r\nSystem initialized successfully!\r\n");
    uart0_puts("UART0 ready at 115200 bps\r\n");
    uart0_puts("Please enter '1' or '2' to execute different functions:\r\n");
    uart0_puts("Press 1: Left Running LEDs\r\n");
    uart0_puts("Press 2: Right Running LEDs\r\n");

    while (1)
    {
        if (UTRSTAT0 & 0x1)
        { // 检测接收就绪
            char c = URXH0;
            uart0_putc(c); // 回显字符

            switch (c)
            {
            case '1':
                uart0_puts("\r\nLeft Running...");
                led_run_left(5);
                break;
            case '2':
                uart0_puts("\r\nRight Running...");
                led_run_right(5);
                break;
            default:
                uart0_puts("\r\nInvalid input!");
                break;
            }
        }
    }
    return 0;
}