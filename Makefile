C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

# Change this if your cross-compiler is somewhere else
CC = /home/rs/Desktop/elf_compiler/i686-elf-tools-linux/bin/i686-elf-gcc
LD = /home/rs/Desktop/elf_compiler/i686-elf-tools-linux/bin/i686-elf-ld
GDB = /home/rs/Desktop/elf_compiler/i686-elf-tools-linux/bin/i686-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS =  -m32 -nostdlib -nostdinc -Wall -Wextra -fno-builtin -nostartfiles -nodefaultlibs -fno-stack-protector
LDFLAGS = -m elf_i386 -T link.ld
#		 -Wall -Wextra -Werror
#CFLAGS = -nostdlib -nostdinc -fno-builtin -fno-stack-protector
# First rule is run by default
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ $^ --oformat binary ${LDFLAGS}

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ $^ ${LDFLAGS}

run: os-image.bin
	qemu-system-x86_64 -hda os-image.bin -m 128M

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
