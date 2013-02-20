#ifndef _RASTER_ICON_HEADER_H_
#define _RASTER_ICON_HEADER_H_

#include <esic/esic.h>

typedef struct _RasterIconHeader {
	DWORD signature;
	WORD  version;
	WORD  width;
	WORD  height;
	BYTE clrpalette;
	BYTE bpp; 
} RasterIconHeader;

// foo

#endif /* _RASTER_ICON_HEADER_H_ */