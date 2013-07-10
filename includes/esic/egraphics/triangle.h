/**
 * \file triangle.h
 */
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <esic/egraphics/vertice.h>
#include <esic/egraphics/shape.h>

typedef struct _Triangle {
	Shape shape;
	Vertice v2;
	Vertice v3;
} Triangle, *PTriangle;

PTriangle Triangle_constructor(PTriangle self);

/* virtual functions */
/* Object */

void Triangle_destructor(PObject self);
PObject Triangle_clone(PObject self, PObject dst);
BOOL    Triangle_equalsTo(PObject self, PObject dst);
DWORD Triangle_hash(PObject self);

/* Shape */

void Triangle_paint(PShape self, PPainter painter);
void Triangle_translateTo(PShape self, SDWORD x, SDWORD y);

#define TRIANGLE(x) ((PTriangle)x)

#endif /* _TRIANGLE_H_ */
