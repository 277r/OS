ENTRY(_start)
SECTIONS
{
  .text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
	}
  .text 0x100000 :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(4096);
  }

  

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    . = ALIGN(4096);
  }


  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;
} 