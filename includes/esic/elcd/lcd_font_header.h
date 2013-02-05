#ifndef _LCD_FONT_HEADER_H_
#define _LCD_FONT_HEADER_H_

#include <stdio.h>
#include <esic/e11_types.h>

typedef struct _LcdFontHeader {
	DWORD			signature;				/* Signature of a lcd font file (must be: 0x0151c51c) */
	WORD			version;				/* To support extensions? */
	WORD			number_of_tables;		/* Number of tables */
} LcdFontHeader, *PLcdFontHeader;


#endif /* _LCD_FONT_HEADER_H_ */