.text
.global led_control /* 声明一个全局的标号 */
led_control:
ldr r0, =0xE0200280 /* GPJ2CON 寄存器的地址为 0xE0200280 */
ldr r1, [r0] /* 读出 GPJ2CON 寄存器原来的值*/
bic r1, r1, #0x000f /* bit[3:0]清零 */
orr r1, r1, #0x0001 /* 设置 GPJ2_0[3:0]=0b0001 */
str r1, [r0]
ldr r0, =0xE0200284 /* 写入 GPJ2CON，配置 GPJ2_0 为输出引脚*/
ldr r1, [r0] /* 读出 GPC0DAT 寄存器原来的值*/
bic r1, r1, #0x1 /* bit[0]清零 */
orr r1, r1, #0x0 /* bit[0]=0，用于点亮第 1 颗LED */
str r1, [r0] /* 写入 GPJ2DAT，GPJ2_0 输出高电平*/
halt_loop:
    b halt_loop /*不让程序跑飞*/