#include "common.h"
#include "drivers/screen.h"
#include "libc/string.h"
extern void panic(const char *message, const char *file, u32 line) {
	asm volatile("cli");
	kprint("PANIC! (");
	kprint(message);
	kprint(") at ");
	kprint(file);
	char *ascii_line;
	int_to_ascii(line, ascii_line);
	kprint(":");
	kprint(ascii_line);
	kprint("\n");
	for(;;);
}

