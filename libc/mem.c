#include "mem.h"
#include "../drivers/screen.h"
void memory_copy(u8 *source, u8 *dest, int nbytes) {
	int i;
	for (i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

void memory_set(u8 *dest, u8 val, u32 len) {
	kprint("memory-setted\n");
	u8 *temp = (u8 *)dest;
	kprint("detsed");
	for (;len!=0;len--) *temp++ = val;
	kprint("done");
}

u32 free_mem_addr = 0x10000; // this really should be computed at link time but this'll do for now, rememver that our kernel is at 0x1000
u32 kmalloc_int(u32 size, int align, u32 *phys_addr) {
	if(align == 1 && (free_mem_addr & 0xFFFFF000)) {
		free_mem_addr &= 0xFFFFF000;
		free_mem_addr += 0x1000;
	}
	if (phys_addr) *phys_addr = free_mem_addr;

	u32 ret = free_mem_addr;
	free_mem_addr += size;
	return ret;
}
