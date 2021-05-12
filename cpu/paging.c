#include "../common.h"
#include "types.h"
#include "paging.h"

/*
	explanation:
	all memory available gets split up into 4KiB blocks
	all pages are in order
	if we want bigger pages we need 1024 pages in a row (like 4MiB instead of 4KiB)
	these pages get a certain bit changed that shows its reserved by another page






example:
512KiB of ram
4KiB of ram/page 
lets say page size = 16 bytes


2048KiB / 4KiB = 512 pages needed

512 * 16 = 8192 bytes needed for all pages

512KiB -8192 = 510KiB of actual space

so we need something against that





*/

// here will be some tests where i try software paging

long long *aop = (long long*)0x20000;

// place that the first page points to

unsigned long fpll = sizeof(bruh_t);
void *fpl;


// 0x1000 = 4KiB
bruh_t *page = (bruh_t*)0x21000;
// create pages by memory size, divide whole memory into 4KiB blocks and then decide what amount to reserve for the page handler stuff
void init_pages(long long memSize)
{	
	// remove the 0x21000 bytes already in use
	memSize -= 0x21000;

	// divide into 4K chunks
	*aop = memSize / 4096;

	// remove the pages from memory because we can't use that space
	memSize -= (*aop) * sizeof(bruh_t);
	// now we have the full amount of ram that we can actually use, the amount needed for paging is not in this value
	*aop = memSize / 4096;

	// set fpl to point to the first location in memory that we cna actually use, after all the stuff
	fpll *= *aop;
	fpll += 0x21000;
	fpl = (void*) fpll;

	for (long long x = 0; x < *aop; x++)
	{
		// *(page + x) = 0
		page[x].h = 0;
	}
	for (long long x = 0; x < *aop; x++){
		// make pages go from every 4 KiB
		page[x].loc = (void*) (fpl + (4096 * x));
		page[x].bytesUsed = 0;
		page[x].currentINdex = 0;

	}
	return;
}

void* getFPL(){
	
	return fpl;
}

void delete_page(long long pageID){
	page[pageID].h = 0;
	// store


}

void delete_pages_by_pid(long pid){
	for (long long x = 0; x < *aop; x++){
		if (page[x].proc == pid){
			delete_page(x);
		}
	}
}


// return code 1 = success, 2 = fail;
void* create_page(char size){
	// 1 = 4KiB, 2 = 4MiB, 3 = 4GiB
	if (size == 1){
		for (long long x = 0; x < *aop; x++){
			// and h with 0xF to get last 4 bytes
			if(page[x].h & 0xF == 0){
				// set to used
				page[x].h = 1;
				// return locaton of pointer
				return page[x].loc;
			}
		}
	}
	else if (size == 2)
	{
		// much more complicated
		
	}
}
/*
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
	for (i=0;i <  INDEX_FROM_BIT(nframes); i++);
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
	if(dir->tables[table_idx]) {
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
*/