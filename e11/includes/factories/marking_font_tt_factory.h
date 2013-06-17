#ifndef _MARKING_FONT_FACTORY_H_
#define _MARKING_FONT_FACTORY_H_

#include <libsic/marking/marking_font_tt.h>
#include <esic/etools/hashtable.h>

#define MARKING_FONT_TT_EXT	".sicfntt"
#define PATH_MARKING_FONTS "system/marking_fonts/"

#include <esic/eapi/heap.h>

void MarkingFontTTFactory_init();
void MarkingFontTTFactory_destroy();
PMarkingFontTT MarkingFontTTFactory_getMarkingFontTT(const char* font_name);

#endif /* _MARKING_FONT_FACTORY_H_ */