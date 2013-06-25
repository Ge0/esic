#ifndef _PAINTER_H_
#define _PAINTER_H_

#include <esic/object.h>
#include <esic/eresources/raster_font.h>
#include <esic/egraphics/renderer.h>

typedef struct _ClipPainter {
	DWORD x;
	DWORD y;
	DWORD width;
	DWORD height;
} ClipPainter;

typedef struct _Painter *PPainter;
typedef struct _Painter {
	Object object;
	ClipPainter clip;
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

void Painter_drawLine(PPainter self, DWORD x1, DWORD y1, DWORD x2, DWORD y2);
void Painter_drawRectangle(PPainter self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD border_color);
void Painter_drawTriangle(PPainter self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD border_color);
void Painter_drawBuffer(PPainter self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer);
void Painter_drawPixel(PPainter self, DWORD x ,DWORD y);
void Painter_drawString(PPainter self, DWORD x, DWORD y, const char* text);

#define PAINTER(x) ((PPainter)x)

#endif /* _PAINTER_H_ */