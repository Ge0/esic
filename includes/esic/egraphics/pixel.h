#ifndef _PIXEL_H_
#define _PIXEL_H_


#include <esic/egraphics/vertice.h>
#include <esic/egraphics/shape.h>

typedef struct _Pixel *PPixel;

typedef struct _Pixel {
	Shape shape;
	Vertice coords;
	DWORD color;
} Pixel;

PPixel Pixel_constructor(PPixel self);

/* Virtual functions */
/* Object */
void Pixel_destructor(PObject self);
PObject Pixel_clone(PObject self, PObject dst);
BOOL Pixel_equalsTo(PObject self, PObject dst);
DWORD Pixel_hash(PObject self);
DWORD Pixel_type(PObject self);

/* Shape */
void Pixel_paint(PShape self, PPainter painter);

#define PIXEL(x) ((PPixel)x)

#endif /* _PIXEL_H */