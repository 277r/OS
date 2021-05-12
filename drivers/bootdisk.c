
#include "bootdisk.h"

long long kernel_offset;

void set_kernel_offset(){
    
}


void loadsectors(int amount){
    // assembly code:
    /*
    mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
	; max amount of sectors to load = 0x80, loading more sectors might make our bios work again
	mov dh, 0x16 
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret
    */
}