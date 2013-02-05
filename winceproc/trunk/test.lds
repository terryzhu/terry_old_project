
MEMORY
{
	CODE (rx) : ORIGIN = 0x00000000, LENGTH = 0x00100000
	DATA (rw) : ORIGIN = 0x0100000, LENGTH = 0x00100000
}

SECTIONS
{
	. = 0x0;
	.text :	{ *(.text) }
	. = 0x100000;
	.data : { *(.data) }
	.bss  : { *(.bss)  }
}
