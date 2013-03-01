/**
 * \file raster_icon_header.h
 */
#ifndef _RASTER_ICON_HEADER_H_
#define _RASTER_ICON_HEADER_H_

#include <esic/esic.h>

#define RASTER_ICON_MAGIC_WORD 0x51C1C010

typedef struct _RasterIconHeader {
	DWORD signature;
	WORD  version;
	WORD  width;
	WORD  height;
	BYTE clrpalette;
	BYTE bpp; 
} RasterIconHeader;


#endif /* _RASTER_ICON_HEADER_H_ */