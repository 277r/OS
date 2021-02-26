#ifndef TYPES_H
#define TYPES_H

/* Instead of using 'chars' to allocate non-character bytes,
 * we will use these new type with no semantic meaning */
typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

// SMAP entry structure

typedef struct SMAP_entry
{

	long long Base; // base address long long
	long long Length;
	long Type; // entry Type
	long ACPI; // extended
} __attribute__((packed)) SMAP_entry_t;


// this is for some test stuff i'm currently doing, can probably be ignored
typedef struct MEM_PAGE {

	// first 4 bytes = size
	// last 4 bytes = used / not;

	// 0 = 4KiB, 1 = 4MiB, 2 = 4GiB although that will probably never happen
	// 0 = free, 1 = used, 2 = space used by other page (i.e. larger page than 4KiB)
	char h;

	// process that uses this page
	long proc;
	// location in ram this table is;
	long long loc;
	// virtual address
	long long virt_addr_start;
} bruh_t;

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
