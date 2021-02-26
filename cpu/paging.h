#ifndef PAGING_H
#define PAGING_H
#include "../common.h"
#include "../cpu/types.h"
#include "../cpu/isr.h"
#include "../libc/mem.h"
#endif

typedef struct page {
	u32 present : 1;
	u32 rw : 1;
	u32 user : 1;
	u32 accessed : 1;
	u32 dirty : 1;
	u32 unused : 7;
	u32 frame : 20;
}__attribute__((packed)) page_t;

typedef struct page_table {
	page_t pages[1024];
}__attribute__((packed)) page_table_t;

typedef struct page_dir {
	// array of pointers to pagetables
	page_table_t *tables[1024];
	//array of pointers to the above but gives a *physical* location
	u32 tablesPhysical[1024];
	// the physical addr of tablesPhysical
	u32 physicalAddr;
}__attribute__((packed)) page_dir_t;


/*
// enables paging?
void initialise_paging();
// loads the specified page dir into the CR3 register
void switch_page_dir(page_dir_t *new_page);
// gets a pointer to the page wanted
// if make == 1 , if the page-table in which this thing should reside doesnt exist it is created.
page_t *get_page(u32 addr, int make, page_dir_t *dir);
// handler for page faults
void page_fault(registers_t regs);
*/