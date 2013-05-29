/**
 * \file triangle.h
 */
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <esic/egraphics/vertice.h>
#include <esic/egraphics/shape.h>

typedef struct _Triangle {
	Shape shape;
	Vertice v1;
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

void Triangle_paint(PShape self, PAbstractPainter painter);



/* Object vtable declaration */
/*
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Triangle_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
*/

/* Shape vtable declaration */
/*
#define SHAPE_VFUNCTION(return_type, function_name, arguments) \
	return_type Triangle_##function_name##arguments;

	SHAPE_VIRTUAL_FUNCTIONS
#undef SHAPE_VFUNCTION
*/

#define TRIANGLE(x) ((PTriangle)x)

#endif /* _TRIANGLE_H_ */
