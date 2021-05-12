#include "../lib/types.h"

typedef struct {
	// information of the
	char info;
	// the pointer to where the memory is allocated
	void* pointer;
	// amount of memory allocated in bytes
	size_t size;

} mementry;


// we hope that stack memory doesn't exceed 32kb, we can change this later if we want to
void *mem_entry_location = 0x8000;
size_t mementcount = 0;


// initialize the memory thing
void mem_init(){
	mem_entry_location = 0x8000;
	mementcount = 100;
	// set the memory entry table position, this table can be moved anywhere
	((mementry*)(mem_entry_location))->pointer = 0x8000;
	((mementry*)(mem_entry_location))->size = sizeof(mementry) * 100;

	// set all memory to 0 
	
}



// return memory
void *malloc(int size){
	
}