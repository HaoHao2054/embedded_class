/*start.S*/
.global _start
_start:
    ldr sp, =0xD0020010
    bl led_control // 跳转执行 LED 控制
halt:
    b halt