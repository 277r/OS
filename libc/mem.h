#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"
#define kmalloc(x) kmalloc_int(x,0,0)
#define kmalloc_a(x) kmalloc_int(x,1,0)
#define kmalloc_p(x,y) kmalloc_int(x,0,y)
#define kmalloc_ap(x,y) kmalloc_int(x,1,y)
void memory_copy(u8 *source, u8 *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);
u32 kmalloc_int(u32 size, int align, u32 *phys_addr);
#endif
