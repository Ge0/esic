#ifndef _PAINTER_H_
#define _PAINTER_H_

#include <esic/object.h>
#include <esic/eresources/raster_font.h>
#include <esic/egraphics/renderer.h>

typedef struct _Painter *PPainter;
typedef struct _Painter {
	Object object;
	struct {
		DWORD x;
		DWORD y;
		DWORD width;
		DWORD height;
	} clip;
	DWORD color;
	PRasterFont font;
	PRenderer renderer;
} Painter;

PPainter Painter_constructor(PPainter self);

/* Virtual functions */
/* Object */
void Painter_destructor(PObject self);
PObject Painter_clone(PObject self, PObject dst);
BOOL Painter_equalsTo(PObject self, PObject dst);
DWORD Painter_hash(PObject self);
DWORD Painter_type(PObject self);

#define PAINTER(x) ((PPainter)x)

#endif /* _PAINTER_H_ */