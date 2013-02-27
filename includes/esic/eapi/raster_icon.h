/**
 * \file raster_icon.h
 */
#ifndef _RASTER_ICON_H_
#define _RASTER_ICON_H_

#define RASTER_ICON_MAGIC_WORD 0x51C1C010
#define RASTER_ICON_FILENAME_LENGTH 32

#include <esic/object.h>
#include <esic/eapi/raster_icon_header.h>

typedef struct _RasterIcon {
	Object object;
	RasterIconHeader header;
	BYTE* data;
} RasterIcon, *PRasterIcon;

PRasterIcon RasterIcon_constructor(PRasterIcon self);

/* Virtual functions */
/* Object */
void RasterIcon_destructor(PObject self);
PObject RasterIcon_clone(PObject self, PObject dst);

#endif /* _RASTER_ICON_H_ */