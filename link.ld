ENTRY(let_it_start)

SECTIONS {
	. = 0x00000000;
	.text ALIGN (0x1000) :
	{
		*(.text)
	}

	.rodata ALIGN (0x1000) :
	{
		*(.rodata*)
	}

	.data ALIGN (0x1000) :
	{
		*(.data)
	}

	.bss ALIGN (0x1000) :
	{
		*(COMMON)
		*(.bss)
	}
}
