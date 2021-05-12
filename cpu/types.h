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

	unsigned long BaseL; // idk
	unsigned long BaseH; // base address long long
	unsigned long LengthL;
	unsigned long LengthH;
	unsigned long Type; // entry Type
	unsigned long ACPI; // extended
} SMAP_entry_t;


// this is for some test stuff i'm currently doing, can probably be ignored
typedef struct {

	// first 4 bytes = size
	// last 4 bytes = used / not;

	// 0 = 4KiB, 1 = 4MiB, 2 = 4GiB although that will probably never happen
	// 0 = free, 1 = used, 2 = space used by other page (i.e. larger page than 4KiB)
	char h;

	// acts as index to if we can fill up this page more
	unsigned long currentINdex;
	// amount that is used, can increase/decrease 
	unsigned long bytesUsed;
	// process that uses this page
	long proc;
	// location in ram this table is;
	void* loc;
	} bruh_t;

/* Struct which aggregates many registers */
typedef struct
{
	u32 ds;										/* Data segment selector */
	u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
	u32 int_no, err_code;						/* Interrupt number and error code (if applicable) */
	u32 eip, cs, eflags, useresp, ss;			/* Pushed by the processor automatically */
} registers_t;



#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
