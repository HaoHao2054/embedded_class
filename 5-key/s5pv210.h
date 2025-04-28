
#ifndef __S5PV210_H__
#define __S5PV210_H__

#include <stdint.h>

/***************************** GPIO 寄存器定义 *****************************/
/* GPJ2 控制寄存器 (LED) */
#define GPJ2CON      (*(volatile uint32_t *)0xE0200280)
#define GPJ2DAT      (*(volatile uint32_t *)0xE0200284)
#define GPJ2PUD      (*(volatile uint32_t *)0xE0200288)

/* GPH0 控制寄存器 (按键) */
#define GPH0CON      (*(volatile uint32_t *)0xE0200C00)
#define GPH0DAT      (*(volatile uint32_t *)0xE0200C04)
#define GPH0PUD      (*(volatile uint32_t *)0xE0200C08)

/************************** 外部中断控制寄存器 **************************/
/* 外部中断控制寄存器组 (EXT_INT_0) */
#define EXT_INT_0_CON       (*(volatile uint32_t *)0xE0200E00)  // 中断触发方式配置
#define EXT_INT_0_MASK      (*(volatile uint32_t *)0xE0200F00)  // 中断屏蔽控制
#define EXT_INT_0_PEND      (*(volatile uint32_t *)0xE0200F40)  // 中断挂起状态

/* 外部中断触发方式定义 (EXT_INT_0_CON) */
#define EXT_INT_MODE_LOW_LEVEL       (0x0)  // 低电平触发
#define EXT_INT_MODE_HIGH_LEVEL      (0x1)  // 高电平触发
#define EXT_INT_MODE_FALLING_EDGE    (0x2)  // 下降沿触发
#define EXT_INT_MODE_RISING_EDGE     (0x3)  // 上升沿触发
#define EXT_INT_MODE_BOTH_EDGE       (0x4)  // 双边沿触发

/************************** VIC 中断控制器寄存器 **************************/
#define VIC0_BASE           0xF2000000

/* VIC0 寄存器组 */
#define VIC0IRQSTATUS       (*(volatile uint32_t *)(VIC0_BASE + 0x000)) // IRQ状态
#define VIC0INTSELECT       (*(volatile uint32_t *)(VIC0_BASE + 0x00C)) // 中断类型选择
#define VIC0INTENABLE       (*(volatile uint32_t *)(VIC0_BASE + 0x010)) // 中断使能
#define VIC0INTENCLEAR      (*(volatile uint32_t *)(VIC0_BASE + 0x014)) // 中断禁用
#define VIC0VECTADDR0       (*(volatile uint32_t *)(VIC0_BASE + 0x100)) // 向量地址0
#define VIC0VECTADDR3       (*(volatile uint32_t *)(VIC0_BASE + 0x10C)) // 向量地址3(EINT3)
#define VIC0ADDRESS         (*(volatile uint32_t *)(VIC0_BASE + 0xF00)) // 当前ISR地址

/* 中断号定义 */
#define INT_EINT3           (3)  // 外部中断3

/***************************** 硬件引脚定义 *****************************/
/* LED 引脚定义 (低电平点亮) */
#define LED1                (0x1 << 0)  // GPJ2_0
#define LED2                (0x1 << 1)  // GPJ2_1
#define LED3                (0x1 << 2)  // GPJ2_2
#define LED4                (0x1 << 3)  // GPJ2_3
#define ALL_LEDS            (LED1 | LED2 | LED3 | LED4)

/* 按键引脚定义 */
#define SW5_PIN             (1 << 3)    // GPH0_3 (EINT3)
#define SW6_PIN             (1 << 4)    // GPH0_4 (EINT4)

/***************************** 函数声明 *****************************/
void delay(volatile unsigned long dly);
void init_key(void);
void led_init(void);
void clear_int_pend(void);
void init_int(void);
void clear_vectaddr(void);
unsigned long get_irgstatus(void);



#endif /* __S5PV210_H__ */