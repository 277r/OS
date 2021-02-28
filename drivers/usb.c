#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#include "usb.h"

static void mouse_callback(registers_t regs)
{
	u8 scancode = port_byte_in(0x60);
	printHex(scancode);

	return;
}

void init_usb()
{
	register_interrupt_handler(IRQ9, mouse_callback);
	return;
}
