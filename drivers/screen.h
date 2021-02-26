
#ifndef SCREEN_H
#define SCREEN_H
#include "../cpu/types.h"
#define kprint(x) kprint_at(x,-1,-1)
#define VGA_ADDR 0xb8000


#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#endif

/* Public kernel API */
void clear_screen();
void kprint_at(char *message, int col, int row);
//void kprint(char *message);
void kprint_backspace();


void printHex(unsigned char ionput);
void printInt(long long in);
