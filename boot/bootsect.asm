[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; The same one we used when linking the kernel
let_it_start:
	
	
	mov ah, 0x0e ; tty mode
	mov al, 'H'
	int 0x10
	mov al, 'e'
	int 0x10
	mov al, 'l'
	int 0x10
	int 0x10 ; 'l' is still on al, remember?
	mov al, 'o'
	int 0x10





	mov [BOOT_DRIVE], dl ; Remember that the BIOS sets us the boot drive in 'dl' on boot
	mov bp, 0x9000
	mov sp, bp

	call load_kernel ; read the kernel from disk

	; first get this done or stuff might not work
	; try do_e801 first because i don't understand e820 SMAP yet
	;call do_e820 ; get the memory stuff done

	call do_e801; find the first 4 possible GiB

	call do_e820; other ram detecting method for more than 4GiB

	call switch_to_pm ; disable interrupts, load GDT,  etc. Finally jumps to 'BEGIN_PM'
	jmp $ ; Never executed

%include "boot/load_disk.asm"
%include "boot/gdt.asm"

%include "boot/switchto32.asm"

[bits 16]
load_kernel:
	mov bx, KERNEL_OFFSET ; Read from disk and store in 0x1000
	; max amount of sectors to load = 0x80, loading more sectors might make our kernel work again
	mov dh, 0x17
	mov dl, [BOOT_DRIVE]
	call disk_load
	ret

[bits 16]
do_debug:
	mov ah, 0x80
	mov al, 'H'
	int 0x10
	ret

; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; note: initially di is 0, be sure to set it to a value so that the BIOS code will not be overwritten. 
;       The consequence of overwriting the BIOS code will lead to problems like getting stuck in `int 0x15`
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
; can't go above 0xFFFF because exceeds 16 bit stuff smh
mmap_ent equ 0xFF00           ; the number of entries will be stored at 0xFF00
[bits 16]
do_e820:
	mov di, 0x8004          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	clc		; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret


[bits 16]
do_e801:
	; these 2 might fuck something up maybe

	XOR ECX, ECX
	XOR EDX, EDX
	MOV AX, 0xE801
	INT 0x15		; request upper memory size
	JC SHORT .ERR
	CMP AH, 0x86		; unsupported function
	JE SHORT .ERR
	CMP AH, 0x80		; invalid command
	JE SHORT .ERR
	JCXZ .USEAX		; was the CX result invalid?
 
	MOV AX, CX
	MOV BX, DX
	
	
	mov [0x8000], DX; copy to 0x8000
	ret;
.USEAX:
	; AX = number of contiguous Kb, 1M to 16M
	; BX = contiguous 64Kb pages above 16M
	mov [0x8000], BX; copy to 0x8000
	ret;
.ERR:
	mov ah, 0x0e ; tty mode
	mov al, 'H'
	int 0x10
	hlt;
	ret;





[bits 32]
BEGIN_PM:
	call KERNEL_OFFSET ; Give control to the kernel
	jmp $ ; Stay here when the kernel returns control to us (if ever)


BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
; padding
times 510 - ($-$$) db 0
dw 0xaa55


