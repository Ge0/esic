#ifndef _RASTER_FONT_H_
#define _RASTER_FONT_H_

#define RASTER_FONT_MAGIC_WORD			0x0151C51C
#define RASTER_FONT_CURRENT_VERSION	0x0100
#define RASTER_FONT_FILENAME_LENGTH	32

#include <stdio.h>
#include <esic/etools/vector.h>

#include <esic/etools/container.h>
#include <esic/eapi/raster_font_header.h>
#include <esic/eapi/raster_font_table.h>

/*
#ifdef _DEBUG
#include <esic/heap.h>
#endif
*/

typedef struct _RasterFont {
	Object object;
	char name[RASTER_FONT_FILENAME_LENGTH];
	RasterFontHeader header;
	Vector tables;			/* Will handle RasterFontTable structures */
} RasterFont, *PRasterFont;


PRasterFont	RasterFont_constructor(PRasterFont self);
char*		RasterFont_getCharacterData(PRasterFont self, DWORD utf8_code, DWORD* size, BYTE* unit_width, BYTE* unit_height);

/* Virtual functions */
void		RasterFont_destructor(PObject self);
PObject		RasterFont_clone(PObject self, PObject dst);




#endif /* _RASTER_FONT_H_ */