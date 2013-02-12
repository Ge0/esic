#ifndef _RASTER_FONT_TABLE_H_
#define _RASTER_FONT_TABLE_H_

#include <esic/object.h>
#include "raster_font_table_header.h"


//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif


typedef struct _RasterFontTable {
	Object object;
	RasterFontTableHeader header;
	void* data;
} RasterFontTable, *PRasterFontTable;

PRasterFontTable RasterFontTable_constructor(PRasterFontTable self);

/* Virtual functions */
void RasterFontTable_destructor(PObject self);
PObject RasterFontTable_clone(PObject self, PObject dst);



#endif /* _RASTER_FONT_TABLE_H_ */
