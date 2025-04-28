.text
.global _start           /* 声明全局标号 */
.global IRQ_handle

_start:
    mrs r0, cpsr
    bic r0, r0, #0x00000080  /* 使能IRQ中断 bit[7]=0 */
    msr cpsr, r0
    bl main
halt_loop:
    b halt_loop          /* 死循环，防止程序跑飞 */

IRQ_handle:
    sub lr, lr, #4       /* 修正返回地址 */
    stmdb sp!, {r0-r12, lr}  /* 保存寄存器现场 */
    bl irq_handler       /* 调用中断服务函数 */
    ldmia sp!, {r0-r12, pc}^ /* 恢复现场并返回（^表示恢复CPSR） */