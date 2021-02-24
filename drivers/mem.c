#include "mem.h"



/*
long getMemSize(){
	
	long x = 0;

/* this does NOT seem to work
	asm volatile(
		".error:\n\t"
		"ret;");
	asm volatile(
		".use_ax:\n\t"
		"ret;");
	asm volatile(
		"push %ecx;\n"
		"push %edx;\n"
		"xor %ecx, %ecx;\n"
		"xor %edx, %edx;\n"
		"mov %ax, 0xe801;\n"
		"int $0x15;\n"
		"jc .error;\n"
		"cmp %ah, 0x86;\n"
		"je .error;\n"
		"cmp %ah, 0x80;\n"
		"je .error;\n"
		"jcxz .use_ax;\n"
		"mov %ax, %cx;\n"
		"mov %bx, %dx;\n");
*/
	/* also does NOT seem to work
	asm(
		".ERR:\n\t"
		"ret;");

	asm(
		"XOR %CX, %CX;"
		"XOR %DX, %DX;"
		"XOR %AX, %AX;"
		"MOV %AX, 0xE820;"
		"INT $0x15;"
		"JC .ERR;"
		"CMP %AH, 0x86;"
		"JE .ERR;"
		"CMP %AH, 0x80;"
		"JE .ERR;"
		"JCXZ .USEAX;"
		"MOV %AX, %CX;"
		"MOV %BX, %DX;"
		".USEAX:");
		

	asm(
		"XOR %CX, %CX;"
		"XOR %DX, %DX;"
		"XOR %AX, %AX;"
		"MOV %EAX, 0xe820;"
		"INT $0x15;");

	asm("movl %%edx,%0"
			: "=r"(x));
	return x;
}*/

/*
long long getMemSizeExtended()
{
	long long int x;

	__asm__(
		"push	ecx;"
		"push	edx;"
		"xor	ecx, ecx;"
		"xor	edx, edx;"
		"mov	ax, 0xe801;"
		"int	0x15;"
		"jc	.error;"
		"cmp	ah, 0x86		;"
		"je	.error;"
		"cmp	ah, 0x80		;"
		"je	.error;"
		"jcxz	.use_ax			;"
		"mov	ax, cx			;"
		"mov	bx, dx;");

	asm("\t movl %%ebx,%0"
		: "=r"(x));
	return x;
}*/





// we'll just use some copy paste thing from online

__asm__(".code16gcc\n");

// SMAP entry structure

typedef struct SMAP_entry
{

	unsigned long BaseL; // base address uint64_t
	unsigned long BaseH;
	unsigned long LengthL; // length uint64_t
	unsigned long LengthH;
	unsigned long Type; // entry Type
	unsigned long ACPI; // extended

} __attribute__((packed)) SMAP_entry_t;

// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t *buffer, int maxentries)
{
	unsigned long contID = 0;
	int entries = 0, signature, bytes;
	do
	{
		__asm__ __volatile__("int  $0x15"
							 : "=a"(signature), "=c"(bytes), "=b"(contID)
							 : "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150)
			return -1; // error
		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
			// ignore this entry
		}
		else
		{
			buffer++;
			entries++;
		}
	} while (contID != 0 && entries < maxentries);
	return entries;
}

long getMemSize(){
	SMAP_entry_t *smap = (SMAP_entry_t *)0x1000;
	long smapInt = 0;

	const int smap_size = 0x2000;

	int entry_count = detectMemory(smap, smap_size / sizeof(SMAP_entry_t));
	
	if (entry_count == -1)
	{
		// error - halt system and/or show error message
		return 1;
	}

	else
	{
		// process memory map

		return smapInt;
	}
}
