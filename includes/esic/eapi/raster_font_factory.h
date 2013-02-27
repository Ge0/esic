/**
 * \file raster_font_factory.h
 */
#ifndef _RASTER_FONT_FACTORY_H_
#define _RASTER_FONT_FACTORY_H_
#include <esic/eapi/raster_font.h>

#define PATH_RASTER_FONTS "system/raster_fonts/"

//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif

//PRasterFont RasterFactory_getRasterFont(const char* font_name);

void RasterFontFactory_init();
void RasterFontFactory_destroy();
PRasterFont RasterFontFactory_getRasterFont(const char* font_name);
 // Temp

#endif /* _RASTER_FACTORY_H_ */