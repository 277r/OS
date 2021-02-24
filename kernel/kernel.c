#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../cpu/paging.h"

/*memory driving header*/
#include "../drivers/mem.h"

/*running child proccess*/
int rCP = 0;

void _start() {
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
		kprint("list of commands:\nhelp    lists all commands and their functioning\nhalt    stops the machine");
	}
	else if (strcmp(input, "memsize") == 0){
		kprint("current memsize: ");
		printInt(getMemSize() * 64);
		kprint("kB");
	}

	else {
		kprint("command not found");
	}
	kprint("\n# ");
}
