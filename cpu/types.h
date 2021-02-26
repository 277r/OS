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

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
