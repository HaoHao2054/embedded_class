/*start.S*/
.global _start
_start:
    bl main // 跳转执行 LED 控制代码主函数 main，实现LED控制
halt:
    b halt