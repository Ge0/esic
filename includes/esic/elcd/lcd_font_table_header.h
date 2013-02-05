#ifndef _LCD_FONT_TABLE_HEADER_H_
#define _LCD_FONT_TABLE_HEADER_H_

#include <stdio.h>
#include <esic/e11_types.h>

typedef struct _LcdFontTableHeader {
	DWORD	offset;					/* Offset in the file */
	DWORD	utf8_base_code_point;	/* code point of the first utf8 char in the table */
	BYTE	character_width;		/* width of a character */
	BYTE	character_height;		/* height of a character */
	WORD	number_of_characters;
} LcdFontTableHeader, *PLcdFontTableHeader;


#endif /* _LCD_FONT_TABLE_HEADER_H_ */