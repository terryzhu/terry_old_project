.text
.global _start
_start:
    b   MAINFUNC

MAINFUNC:
	ldr sp, =1024*4	@ 把堆栈指针设置为4K处
	mov	fp, #0			@ 前面没有函数调用，所以把fp清零
	mov r0, #0			@ 把main函数的argc设置为0
	mov r1, #0			@ 把main函数的argv设置为0
	
	bl main					@ 跳转到C语言的main函数
	
halt_loop:
	b halt_loop
