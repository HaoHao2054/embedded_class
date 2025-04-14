#ifndef __S5PV210_H__
#define __S5PV210_H__

#include <stdint.h>

// 时钟相关寄存器
#define APLLCON0   *((volatile unsigned int *)0xE0100100)
#define MPLLCON    *((volatile unsigned int *)0xE0100108)
#define EPLLCON0   *((volatile unsigned int *)0xE0100110)
#define VPLLCON    *((volatile unsigned int *)0xE0100120)
#define CLK_SRC0   *((volatile unsigned int *)0xE0100200)
#define CLK_DIV0   *((volatile unsigned int *)0xE0100300)
#define CLK_DIV1   *((volatile unsigned int *)0xE0100304)
#define CLK_DIV2   *((volatile unsigned int *)0xE0100308)
#define CLK_DIV3   *((volatile unsigned int *)0xE010030C)

// UART相关寄存器
#define GPA0CON    *((volatile unsigned int *)0xE0200000)
#define ULCON0     *((volatile unsigned int *)0xE2900000)
#define UCON0      *((volatile unsigned int *)0xE2900004)
#define UFCON0     *((volatile unsigned int *)0xE2900008)
#define UMCON0     *((volatile unsigned int *)0xE290000C)
#define UTRSTAT0   *((volatile unsigned int *)0xE2900010)
#define UERSTAT0   *((volatile unsigned int *)0xE2900014)
#define UFSTAT0    *((volatile unsigned int *)0xE2900018)
#define UMSTAT0    *((volatile unsigned int *)0xE290001C)
#define UTXH0      *((volatile unsigned char *)0xE2900020)
#define URXH0      *((volatile unsigned char *)0xE2900024)
#define UBRDIV0    *((volatile unsigned int *)0xE2900028)
#define UDIVSLOT0  *((volatile unsigned int *)0xE290002C)

// LED控制宏定义
#define GPJ2CON (*(volatile uint32_t *)0xE0200280)
#define GPJ2DAT (*(volatile uint32_t *)0xE0200284)
#define LED1 (0x1 << 0)
#define LED2 (0x1 << 1)
#define LED3 (0x1 << 2)
#define LED4 (0x1 << 3)
#define ALL_LEDS (LED1 | LED2 | LED3 | LED4)

#endif /* __S5PV210_H__ */