#ifndef _LCD_FONT_TABLE_H_
#define _LCD_FONT_TABLE_H_

#include <esic/object.h>
#include "lcd_font_table_header.h"


//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif


typedef struct _LcdFontTable {
	Object object;
	LcdFontTableHeader header;
	void* data;
} LcdFontTable, *PLcdFontTable;

PLcdFontTable LcdFontTable_constructor(PLcdFontTable self);

/* Virtual functions */
void LcdFontTable_destructor(PObject self);
PObject LcdFontTable_clone(PObject self, PObject dst);


/* Vtables definition */
static const vtable_Object s_font_table_object_vtable = { LcdFontTable_destructor, LcdFontTable_clone, NULL, NULL };

#endif /* _LCD_FONT_TABLE_H_ */
