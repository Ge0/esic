/**
 * \file raster_icon_factory.h
 */
#ifndef _RASTER_ICON_FACTORY_
#define _RASTER_ICON_FACTORY_

#include <esic/eapi/raster_icon.h>

#define PATH_RASTER_ICONS "system/raster_icons/"

#include <esic/eapi/heap.h>

void RasterIconFactory_init();
void RasterIconFactory_destroy();
PRasterIcon RasterIconFactory_getRasterIcon(const char* icon_name);

#endif /* _RASTER_ICON_FACTORY_ */