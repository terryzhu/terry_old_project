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
mov	fp,#0		@ ǰ��û�к������ã����԰�fp����

msr cpsr_c,#0xd2
ldr sp,=0x33000000
msr cpsr_c,#0xdf
ldr sp,=0x34000000
bl InitIRQ
msr cpsr_c,#0x5f


mov r0, #0			@ ��main������argc����Ϊ0
mov r1, #0			@ ��main������argv����Ϊ0
bl main					@ ��ת��C���Ե�main����

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
mrs     r0, cpsr        @���������Լ�д��
and     r0, r0, #0xffffff7f    @ǿ�ƿ����ж�λ
msr     cpsr, R0      @      
ldmia sp!,{r0-r12,pc}@�����^�����ж����ɱ�����







HandleFIQ:
1: b 1b
HandleNotUsed:
1: b 1b
.space 4000,0

