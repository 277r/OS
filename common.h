#include "drivers/screen.h"
#include "libc/string.h"
void panic(char*,char*,u32);
#define PANIC(x) panic(x,__FILE__,__LINE__)
