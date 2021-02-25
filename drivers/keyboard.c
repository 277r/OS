#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

// release key = key + 0x80
// there's something about E0 that i don't understand yet
#define BACKSPACE 0x0E
#define ENTER 0x1C 
#define LSHIFT 0x2A
#define LSHIFTRELEASE 0xAA
#define ESC 1
#define CAPS 0X3A
#define CTRL 0x1D
#define CTRLRELEASE 0x9D
#define ALT 0x38
#define ALTRELEASE 0xB8

static char key_buffer[256];

#define SC_MAX 57
const char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char sc_ascii_uppercase[] = {'?', '?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

int shift = 0;
int caps = 0;
int ctrl = 0;
int alt = 0;
static void keyboard_callback(registers_t regs)
{

	u8 scancode = port_byte_in(0x60);

	// debug only, get key ID from keyUp/keyDown
	//printHex(scancode);

	// scan for shift
	if (scancode == LSHIFT)
	{
		// if shift == 0 then turn into 1, else turn into 0
		shift = 1;
		return;
	}
	else if (scancode == LSHIFTRELEASE)
	{
		// if shift == 0 then turn into 1, else turn into 0
		shift = 0;
		return;
	}

	else if (scancode == CAPS)
	{
		// if caps == 0 then turn into 1, else turn into 0
		caps = caps == 0 ? 1 : 0;
		return;
	}
	else if (scancode == CTRL)
	{
		ctrl = 1;
		return;
	}
	else if (scancode == CTRLRELEASE)
	{
		kprint("\n");

		ctrl = 0;
		return;
	}
	else if (scancode == ALT)
	{
		alt = 1;
		return;
	}
	else if (scancode == ALTRELEASE)
	{
		alt = 0;
		return;
	}

	// control and alt keys control and alternate, these keys can't be used with output
	else if (ctrl || alt)
	{
		if (scancode == BACKSPACE){
			// delete word/row?
			do
				{
					if (!(strlen(key_buffer) > 0))
						return;
					backspace(key_buffer);
					kprint_backspace();
				} while (key_buffer[strlen(key_buffer)-1] != ' ');
		}
		// more stuff
	}
	else if (scancode == ENTER)
	{
		kprint("\n");
		
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else
	{

		// shift & caps = normal alphabet
		// shift XOR caps = capitalized alphabet
		// none = normal alphabet
		if (shift ^ caps)
		{
			if (scancode > SC_MAX){
				return;
			}
			else if (scancode == BACKSPACE)
			{
				if (!(strlen(key_buffer) > 0))
					return;
				backspace(key_buffer);
				kprint_backspace();
			}
			
			else
			{

				char letter = sc_ascii_uppercase[(int)scancode];
				char str[2] = {letter, '\0'};
				append(key_buffer, letter);
				kprint(str);
			}
			UNUSED(regs);
		}
		else
		{
			if (scancode > SC_MAX)
			{
				return;
			}

			else if (scancode == BACKSPACE)
			{
				if (!(strlen(key_buffer) > 0))
					return;
				backspace(key_buffer);
				kprint_backspace();
			}
			
			else
			{

				char letter = sc_ascii[(int)scancode];
				char str[2] = {letter, '\0'};
				append(key_buffer, letter);
				kprint(str);
			}
			UNUSED(regs);
		}
	}
}

void init_keyboard()
{
	register_interrupt_handler(IRQ1, keyboard_callback);
}
