#ifndef _LCD_FONT_H_
#define _LCD_FONT_H_

#define LCD_FONT_MAGIC_WORD			0x0151C51C
#define LCD_FONT_CURRENT_VERSION	0x0100
#define LCD_FONT_FILENAME_LENGTH	32

#include <stdio.h>
#include <esic/etools/vector.h>

#include <esic/etools/container.h>
#include <esic/elcd/lcd_font_header.h>
#include <esic/elcd/lcd_font_table.h>

/*
#ifdef _DEBUG
#include <esic/heap.h>
#endif
*/

typedef struct _LcdFont {
	Object object;
	char name[LCD_FONT_FILENAME_LENGTH];
	LcdFontHeader header;
	Vector tables;			/* Will handle LcdFontTable structures */
} LcdFont, *PLcdFont;


PLcdFont	LcdFont_constructor(PLcdFont self);
char*		LcdFont_getCharacterData(PLcdFont self, DWORD utf8_code, DWORD* size, BYTE* unit_width, BYTE* unit_height);

/* Virtual functions */
void		LcdFont_destructor(PObject self);
PObject		LcdFont_clone(PObject self, PObject dst);




#endif /* _LCD_FONT_H_ */