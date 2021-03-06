#ifndef VIDEO_H
#define VIDEO_H

#include "string.h"

//in/out a byte through port
extern void writeport(dword portNum, dword value);
extern byte readport(dword portNum);
//font binary (see font.bin and font.asm)
extern byte fontbin[752];

void clearScreen(byte color);
//Draw a character in Liberation Mono anywhere on screen, with foreground and background colors given
//Note: x and y are on text grid, 8x16 pixels
void drawChar(char c, int x, int y, byte fg, byte bg);
void vsync();

#endif

