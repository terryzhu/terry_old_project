.text

.global _start


_start:


b Handle_Reset
b HandleUndef
b HandleSWI
b HandlePrefetchAbort
b HandleDataAbort
b HandleNotUsed
b HandleIRQ
b HandleFIQ


Handle_Reset:
 
ldr sp,=0x32000000
mov	fp,#0		@ 前面没有函数调用，所以把fp清零

msr cpsr_c,#0xd2
ldr sp,=0x33000000
msr cpsr_c,#0xdf
ldr sp,=0x34000000
bl InitIRQ
msr cpsr_c,#0x5f


mov r0, #0			@ 把main函数的argc设置为0
mov r1, #0			@ 把main函数的argv设置为0
bl main					@ 跳转到C语言的main函数

loop:
b loop

HandleUndef:
1: b 1b
HandleSWI:
1: b 1b
HandlePrefetchAbort:
1: b 1b
HandleDataAbort:
1: b 1b

HandleIRQ:@ need to write


@@@@@@@@@@@@@@@@@
@stmdb    sp!,{r0-r12,lr}
@   bl        InterruptHandler    
@  ldmia    sp!,{r0-r12,lr}
@ subs    pc,lr,#4
@@@@@@@@@@@@@@@@@


sub lr,lr,#4
stmdb sp!,{r0-r12,lr}
ldr lr,=int_return
ldr pc,=InterruptHandler
int_return:
mrs     r0, cpsr        @以下三行自己写的
and     r0, r0, #0xffffff7f    @强制开启中段位
msr     cpsr, R0      @      
ldmia sp!,{r0-r12,pc}@必须加^否则中断依旧被屏蔽







HandleFIQ:
1: b 1b
HandleNotUsed:
1: b 1b
.space 4000,0


