
ands    r9, pc, #0xFF000000   @  ; see if we are in flash or in ram
bne     c20              @     ; go ahead if we are already in ram

@ ; This is the loop that perform copying.
ldr     r0, = 0x21000       @    ; offset into the RAM 
add     r0, r0, #0x30000000    @   ; add physical base
mov     r1, r0            @      ; (r1) copy destination
ldr     r2, =0x0          @      ; (r2) flash started at physical address 0
ldr     r3, =0x10000        @    ; counter (0x40000/4)
b10:
ldr     r4, [r2], #4
str     r4, [r1], #4
subs    r3, r3, #1
bne     b10

@   ; Restart from the RAM position after copying.
mov pc, r0




c20:



这段代码,如老朽枯木,尽归尘土:

ands    r9, pc, #0xFF000000   @  ; see if we are in flash or in ram
bne     c20


ldr sp, =1024*4	@ 把堆栈指针设置为4K处


bl CpyMem

@开中断



ldr sp, =0x32000000	
ldr     r0, =0x30021000
mov pc, r0
c20:


#define  S3C2410_MPLL_200MHZ    ((0x5c<<12)|(0x04<<4)|(0x00))
#define STOP 0
#define START 1
#define MEM_CTL_BASE 0x48000000
#define SDRAM_BASE 0x30000000
#ifndef WTCON
#define WTCON (*(volatile unsigned *)0x53000000)

#endif
void watch_dog()  //关闭看门狗
{
	WTCON = 0;

}
void memery_init()   //设置存储器
{
	unsigned long const    mem_cfg_val[]={ 0x22011110,     //BWSCON
		0x00000700,     //BANKCON0
		0x00000700,     //BANKCON1
		0x00000700,     //BANKCON2
		0x00000700,     //BANKCON3 
		0x00000700,     //BANKCON4
		0x00000700,     //BANKCON5
		0x00018005,     //BANKCON6
		0x00018005,     //BANKCON7
		0x008C07A3,     //REFRESH
		0x000000B1,     //BANKSIZE
		0x00000030,     //MRSRB6
		0x00000030,     //MRSRB7
	};
	int     i = 0;
	volatile unsigned long *p = (volatile unsigned long *)MEM_CTL_BASE;
	for(; i < 13; i++)
		p[i] = mem_cfg_val[i];
}
void copy_stepping_sdram()   //拷贝代码
{
	volatile unsigned long *p = (unsigned long *)SDRAM_BASE;
	unsigned long *step_p=(unsigned long *)0;

	while(step_p<((unsigned long *)4096))
	{
		*p = *step_p;
		step_p ++;
		p++;
	}
}
