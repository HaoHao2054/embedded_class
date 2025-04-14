/*start.S*/
.global _start
_start:
    bl main // 跳转执行控制代码主函数 main
halt:
    b halt