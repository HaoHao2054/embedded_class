.text
.global _start
.global IRQ_handle
_start:
    ldr sp,=0x40000000
    mrs r0 ,cpsr
    bic r0,r0,#0x00000080 //使能IRQ中断bit[7]=0，因为cpsr的bit[7]是IRQ使能
    msr cpsr,r0
    
	bl main
	

halt:
     b halt

IRQ_handle:
    sub lr,lr,#4
    stmdb sp!,{r0-r12,lr}
    bl irq_handler
    ldmia sp!,{r0-r12,pc}^

