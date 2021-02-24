#include "../common.h"
#include "types.h"
#include "paging.h"
u32 *frames;
u32 nframes;
extern u32 free_mem_addr;

// used in the bitset things  this is from 8*4
#define INDEX_FROM_BIT(x) (x/32)
#define OFFSET_FROM_BIT(x) (x%32)

// static func to set a bit in the bitset
static void set_frame(u32 frame_addr) {
	u32 frame = frame_addr/0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}

static void clear_frame(u32 frame_addr) {
	u32 frame = frame_addr/0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

//test if a bit is set
static u32 test_frame(u32 frame_addr) {
	u32 frame = frame_addr/0x1000;
	u32 idx = INDEX_FROM_BIT(frame);
	u32 off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}
//finds first free frame
static u32 first_frame() {
	u32 i,j;
	for (i=0;i <  INDEX_FROM_BIT/*should i precompute this*/(nframes); i++);
	if(frames[i] != 0xFFFFFFFF) {
		//at least one bit is free
		for(j=0;j<32;j++) {
			u32 toTest = 0x1 << j; //TODO: rewrite this
			if(!(frames[i]&toTest)) {
				return  i*32*j;
			}
		}
	}
}

// allocate a frame
void alloc_frame(page_t *page, int is_kernel, int is_writable) {
	if(page->frame != 0) {
		return; // frame was already allocated
	} else {
		u32 idx = first_frame();
		if(idx == (u32)-1) {
			//PANIC("No free frames!");
			kprint("No free frames!");
			for(;;);
		}
		set_frame(idx*0x1000);
		page->present = 1;
		page->rw = (is_writable)?1:0;
		page->user = (is_kernel)?0:1;
		page->frame = idx;
	}
}

// deallocate a frame
void free_frame(page_t *page) {
	u32 frame;
	if (!(frame=page->frame)) {
		return; // didnt actually have an allocated frame
	}
	else {
		clear_frame(frame);// frame is free agen
		page->frame = 0x0;//page now don't have a frame
	}
}
page_dir_t *kernel_dir, *current_dir;
void initialise_paging() {
	// the size of physical memory, for now we assume 16 mb
	u32 mem_end_page = 0x1000000;
	kprint("hello");
	nframes = mem_end_page / 0x1000;
	frames = (u32*)kmalloc(INDEX_FROM_BIT(nframes));
	kprint("kmalloced");
	memory_set(frames,0,INDEX_FROM_BIT(nframes));
	kprint("memsetted");
	kernel_dir = (page_dir_t*)kmalloc_a(sizeof(page_dir_t));
	kprint("kmalloc_a'd");
	memory_set(kernel_dir,0,sizeof(page_dir_t));
	kprint("memset agen");
	current_dir = kernel_dir;
	kprint("HERE");
	// we meed to identity map (phys addr = virt addr) from 0x0 to the end of emory so we can access this transparently as if paging wasn't
	// enabled, NOTE the use of a while loop when we actually change placement_addr by calling kmalloc. A while loop makes it be computed on
	// the fly.
	
	u32 i = 0;
	while(i < free_mem_addr) {
		alloc_frame(get_page(i,1,kernel_dir),0,0);
		i += 0x1000;
	}
	kprint("there");
	register_interrupt_handler(14, page_fault);
	switch_page_dir(kernel_dir);
	kprint("done?");
}
void switch_page_dir(page_dir_t *dir) {
	current_dir = dir;
	asm volatile("mov %0, %%cr3"::"r"(&dir->tablesPhysical));
	u32 cr0;
	asm volatile("mov %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000; //enable paging
	asm volatile("mov %0, %%cr0"::"r"(cr0));
}

page_t *get_page(u32 addr, int make, page_dir_t *dir) {
	// turn the addr into an index
	addr /= 0x1000;
	// find the page table containing this address
	u32 table_idx = addr / 1024;
	if(dir->tables[table_idx]) /* is this table already assigned */{
		return &dir->tables[table_idx]->pages[addr%1024];
	} else if(make) {
		u32 tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memory_set(dir->tables[table_idx],0,0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // present, rw, user
		return &dir->tables[table_idx]->pages[addr%1024];
	}else {
		return 0;
	}
}

void page_fault(registers_t regs) {
	// the fault addr is in cr2
	u32 fault_addr;
	asm volatile("mov %%cr2, %0":"=r"(fault_addr));

	//the error code gives us details of what happened
	int present = !(regs.err_code & 0x1);
	int rw = regs.err_code & 0x2;
	int us = regs.err_code & 0x4;
	int reserved = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;

	//output this thing
	
	kprint("Page fault! (");
	if(present) {kprint("present ");}
	if (rw) {kprint("read_only ");}
	if (us) {kprint("user-mode ");}
	if (reserved) {kprint("reserved ");}
	if (id) {kprint("fetch? ");};
	kprint(") at 0x");
	char* ascii_addr;
	hex_to_ascii(fault_addr, ascii_addr);
	kprint(ascii_addr);
	kprint("\n");
	// PANIC("Page fault");
	for(;;);
}
