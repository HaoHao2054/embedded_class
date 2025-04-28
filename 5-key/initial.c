#include "s5pv210.h"

/**
 * @brief 按键初始化函数
 * 配置GPH0_3为输入模式（SW5按键）
 */
 void init_key(void)
 {
    // 配置GPIO引脚为中断功能
    GPH0CON &= ~(0xFF<<0);
    GPH0CON |= (0xFF<<0);  // key1:bit[3:0]; key2:bit[7:4]
    
    // 配置EXT_INT[0]、EXT_INT[1]为下降沿触发
    EXT_INT_0_CON &= ~(0xFF<<0);
    EXT_INT_0_CON |= 2 | (2<<4);
    
    // 取消屏蔽外部中断
    EXT_INT_0_MASK &= ~0x3;
 }

 /* 清中断挂起寄存器 */
void clear_int_pend() {
    EXT_INT_0_PEND |= 0x3;  // EXT_INT[0]、EXT_INT[1]
}

/* 初始化中断控制器 */
void init_int() {
    // 选择中断类型为IRQ
    VIC0INTSELECT &= ~0x3;  // 外部中断EXT_INT[0]、EXT_INT[1]为IRQ
    // 清VIC0ADDRESS
    VIC0ADDRESS = 0;
    // 设置中断向量地址
    VIC0VECTADDR0 = (unsigned int)IRQ_handle;
    // 使能中断
    VIC0INTENABLE |= 0x3;
}

//清除中断处理函数地址
void clear_vectaddr(void)
{
    VIC0ADDRESS = 0x0;
}

// 读中断状态
unsigned long get_irgstatus(void)
{
    return VIC0IRQSTATUS;
}

 
 /**
  * @brief LED初始化函数
  * 配置GPJ2_0~3为输出模式（LED1~4）
  */
 void led_init(void)
 {
     /* 配置GPJ2_0~3为输出模式 */
     GPJ2CON &= ~(0xFFFF << 0);  // 清除低16位配置
     GPJ2CON |= (0x1111 << 0);   // 设置为输出模式
     
     /* 初始化熄灭所有LED（高电平） */
     GPJ2DAT |= ALL_LEDS;
 }
 
 /**
  * @brief 简单延时函数
  * @param dly 延时参数（值越大延时越长）
  */
 void delay(volatile unsigned long dly)
 {
     volatile unsigned int t = 0xFFFF;
     while(dly--) {
         for(; t > 0; t--);
         t = 0xFFFF;  // 重置计数器
     }
 }
 