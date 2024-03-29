#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../cpu/paging.h"
#include "../libc/math.h"

/*memory driving header*/
#include "../drivers/mem.h"

/*running child proccess*/
int rCP = 0;





void start()
{ 
	// malloc 4KiB
	//strcopy(data, "list of commands:\nhelp    lists all commands and their functioning\nhalt    stops the machine\nmem801    lists memory size up to 4GiB using e801\nmemsize    broken thing that doesn't work\ncalc    not implemented yet\nclear    clears screen");
	clear_screen();
	isr_install();
	irq_install();
	kprint_at("\0",0,0);
	kprint("type help for a list of commands\n# ");
}

void user_input(char *input)
{
	if(strcmp(input, "halt") == 0) {
		kprint("Halting");
		asm volatile("hlt");
	} /*else if(strcmp(input, "PAGE") == 0) {
		u32 phys_addr;
		u32 page = kmalloc(1000,1,&phys_addr);
		char page_str[16] = "";
		hex_to_ascii(page, page_str);
		char phys_str[16] = "";
		hex_to_ascii(phys_addr, phys_str);
		kprint("Page: ");
		kprint(page_str);
		kprint(", physical address: ");
		kprint(phys_str);
		kprint("\n");
	}*/
	else if (strcmp(input, "help") == 0){
		
		kprint("list of commands:\n");
		kprint("help    lists all commands and their functioning\n");
		kprint("halt    stops the machine\n");
		kprint("mem801    lists memory size up to 4GiB using e801\n");
		kprint("memsize    broken thing that doesn't work\n");
		kprint("calc    not implemented yet\n");
		kprint("clear    clears screen ");
		//kprint("list of commands:\nhelp    lists all commands and their functioning\nhalt    stops the machine\nmem801    lists memory size up to 4GiB using e801\nmemsize    broken thing that doesn't work\ncalc    not implemented yet\nclear    clears screen");
	}
	else if (strcmp(input, "memmap") == 0){
			kprint("memsize: ");
			printInt(getMemSize());
			kprint("KiB");
			// print regions too
	}
	else if (strcmp(input, "mem801") == 0)
	{
		// the other memtest that only works up to 4GiB;
		printInt(memSize801() * 64);
		kprint("KiB");
	}
	else if (strcmp(input, "calc") == 0)
	{
		// calculator stuff
	}
	else if (strcmp(input, "clear") == 0)
	{
		clear_screen();
	}
	else {
		kprint("command not found");
	}
	kprint("\n# ");
}
