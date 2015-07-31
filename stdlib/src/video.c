#include "video.h"

//this allows fast reversing of bits in a u8, needed because font bits are backwards
static const byte reverseTable[] = 
{
	0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
	0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
	0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
	0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
	0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
	0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
	0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
	0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
	0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
	0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
	0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
	0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
	0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
	0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
	0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
	0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

void writeAttributeReg(dword index, dword value)
{
	readport(0x3DA);
	writeport(0x3C0, index);
	writeport(0x3C0, value);
}

void enterMode12H()
{
	//write misc reg
	writeport(0x3C2, 0xE3);
	//write seq regs
	writeport(0x3C4, 0x0);
	writeport(0x3C5, 0x3);
	writeport(0x3C4, 0x1);
	writeport(0x3C5, 0x1);
	writeport(0x3C4, 0x2);
	writeport(0x3C5, 0x8);
	writeport(0x3C4, 0x3);
	writeport(0x3C5, 0x0);
	writeport(0x3C4, 0x4);
	writeport(0x3C5, 0x6);
	//unlock CRTC registers
	writeport(0x3D4, 0x3);
	writeport(0x3D5, readport(0x3D5) | 0x80);
	writeport(0x3D4, 0x11);
	writeport(0x3D5, readport(0x3D5) & ~0x80);
	//note: in future make sure the 0x80 bit is set @ 0x3 and unset @ 0x11
	//write CRTC regs
	writeport(0x3D4, 0x0);
	writeport(0x3D5, 0x5F);
	writeport(0x3D4, 0x1);
	writeport(0x3D5, 0x4F);
	writeport(0x3D4, 0x2);
	writeport(0x3D5, 0x50);
	writeport(0x3D4, 0x3);
	writeport(0x3D5, 0x82 | 0x80);
	writeport(0x3D4, 0x4);
	writeport(0x3D5, 0x54);
	writeport(0x3D4, 0x5);
	writeport(0x3D5, 0x80);
	writeport(0x3D4, 0x6);
	writeport(0x3D5, 0x0B);
	writeport(0x3D4, 0x7);
	writeport(0x3D5, 0x3E);
	writeport(0x3D4, 0x8);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0x9);
	writeport(0x3D5, 0x40);
	writeport(0x3D4, 0xA);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0xB);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0xC);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0xD);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0xE);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0xF);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0x10);
	writeport(0x3D5, 0xEA);
	writeport(0x3D4, 0x11 & ~0x80);
	writeport(0x3D5, 0x0C);
	writeport(0x3D4, 0x12);
	writeport(0x3D5, 0xDF);
	writeport(0x3D4, 0x13);
	writeport(0x3D5, 0x28);
	writeport(0x3D4, 0x14);
	writeport(0x3D5, 0x00);
	writeport(0x3D4, 0x15);
	writeport(0x3D5, 0xE7);
	writeport(0x3D4, 0x16);
	writeport(0x3D5, 0x04);
	writeport(0x3D4, 0x17);
	writeport(0x3D5, 0xE3);
	writeport(0x3D4, 0x18);
	writeport(0x3D5, 0xFF);
	//Graphics controller regs
	writeport(0x3CE, 0x0);
	writeport(0x3CF, 0x00);
	writeport(0x3CE, 0x1);
	writeport(0x3CF, 0x00);
	writeport(0x3CE, 0x2);
	writeport(0x3CF, 0x00);
	writeport(0x3CE, 0x3);
	writeport(0x3CF, 0x00);
	writeport(0x3CE, 0x4);
	writeport(0x3CF, 0x03);
	writeport(0x3CE, 0x5);
	writeport(0x3CF, 0x00);
	writeport(0x3CE, 0x6);
	writeport(0x3CF, 0x05);
	writeport(0x3CE, 0x7);
	writeport(0x3CF, 0x0F);
	writeport(0x3CE, 0x8);
	writeport(0x3CF, 0xFF);
	//Attribute controller regs
	writeAttributeReg(0x0, 0x00);
	writeAttributeReg(0x1, 0x01);
	writeAttributeReg(0x2, 0x02);
	writeAttributeReg(0x3, 0x03);
	writeAttributeReg(0x4, 0x04);
	writeAttributeReg(0x5, 0x05);
	writeAttributeReg(0x6, 0x14);
	writeAttributeReg(0x7, 0x07);
	writeAttributeReg(0x8, 0x38);
	writeAttributeReg(0x9, 0x39);
	writeAttributeReg(0xA, 0x3A);
	writeAttributeReg(0xB, 0x3B);
	writeAttributeReg(0xC, 0x3C);
	writeAttributeReg(0xD, 0x3D);
	writeAttributeReg(0xE, 0x3E);
	writeAttributeReg(0xF, 0x3F);
	writeAttributeReg(0x10, 0x01);
	writeAttributeReg(0x11, 0x00);
	writeAttributeReg(0x12, 0x0F);
	writeAttributeReg(0x13, 0x00);
	writeAttributeReg(0x14, 0x00);
	//Might as well leave CRTC regs unlocked but need to restart refreshes
	readport(0x3DA);
	writeport(0x3C0, 0x20);
}

void clearScreen(byte color)
{
	byte* vram = (byte*) 0xA0000;
	writeport(0x3C4, 2);
	//select plane 0
	writeport(0x3C5, 0x1);
	byte plane0 = color & 1 ? 0xFF : 0x0;
	for(int i = 0; i < 0x9600; i++)
	{
		vram[i] = plane0;
	}
	//plane 1
	writeport(0x3C4, 2);
	writeport(0x3C5, 0x2);
	byte plane1 = color & 0x2 ? 0xFF : 0x0;
	for(int i = 0; i < 0x9600; i++)
	{
		vram[i] = plane1;
	}
	//plane 2
	writeport(0x3C4, 2);
	writeport(0x3C5, 0x4);
	byte plane2 = color & 0x4 ? 0xFF : 0x0;
	for(int i = 0; i < 0x9600; i++)
	{
		vram[i] = plane2;
	}
	//plane 3
	writeport(0x3C4, 2);
	writeport(0x3C5, 0x8);
	byte plane3 = color & 0x8 ? 0xFF : 0x0;
	for(int i = 0; i < 0x9600; i++)
	{
		vram[i] = plane3;
	}
}

void drawChar(char c, int x, int y, byte fg, byte bg)
{
	//Bounds checking
	if(x < 0 || x >= 80 || y < 0 || y >= 30)
		return;
	int offset = 16 * (c - 0x21);	//# of bytes into font
	byte* glyph = (byte*) getFontPtr() + offset;
	byte* fb = (byte*) 0xA0000;
	//Plane 0
	writeport(0x3C4, 2);
	writeport(0x3C5, 1);
	byte fgbits = fg & 1 ? 0xFF : 0x0;
	byte bgbits = bg & 1 ? 0xFF : 0x0;
	for(int i = 0; i < 16; i++)
	{
		//Use bits from foreground where font bit is set, and bits from bg when font bit is clear
		//In plane, 80 bytes (1 bit per pixel, 640 pixels) across per pixel row
		//Each char has 16 rows, i represents this from 0 to 15
		fb[80 * (16 * y + i) + x] = (fgbits & reverseTable[glyph[i]]) | (bgbits & ~reverseTable[glyph[i]]);
	}
	//Plane 1
	writeport(0x3C4, 2);
	writeport(0x3C5, 2);
	fgbits = fg & 2 ? 0xFF : 0x0;
	bgbits = bg & 2 ? 0xFF : 0x0;
	for(int i = 0; i < 16; i++)
	{
		//Use bits from foreground where font bit is set, and bits from bg when font bit is clear
		fb[80 * (16 * y + i) + x] = (fgbits & reverseTable[glyph[i]]) | (bgbits & ~reverseTable[glyph[i]]);
	}
	//Plane 2
	writeport(0x3C4, 2);
	writeport(0x3C5, 4);
	fgbits = fg & 4 ? 0xFF : 0x0;
	bgbits = bg & 4 ? 0xFF : 0x0;
	for(int i = 0; i < 16; i++)
	{
		//Use bits from foreground where font bit is set, and bits from bg when font bit is clear
		fb[80 * (16 * y + i) + x] = (fgbits & reverseTable[glyph[i]]) | (bgbits & ~reverseTable[glyph[i]]);
	}
	//Plane 3
	writeport(0x3C4, 2);
	writeport(0x3C5, 8);
	fgbits = fg & 8 ? 0xFF : 0x0;
	bgbits = bg & 8 ? 0xFF : 0x0;
	for(int i = 0; i < 16; i++)
	{
		//Use bits from foreground where font bit is set, and bits from bg when font bit is clear
		fb[80 * (16 * y + i) + x] = (fgbits & reverseTable[glyph[i]]) | (bgbits & ~reverseTable[glyph[i]]);
	}
}

void vsync()
{
	while((readport(0x3DA) & 8) == 0);	//Wait to enter vertical refresh interval
	while((readport(0x3DA) & 8) == 1);			//Wait for vertical refresh interval to end
}
