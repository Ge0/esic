/**
 * \file shape.h
 */
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <esic/object.h>
#include <esic/egraphics/painter.h>
#include <esic/egraphics/vertice.h>

typedef struct _Shape *PShape;

typedef struct _vtable_Shape {
	void (*paint)(PShape self, PPainter painter);
	void (*translateTo)(PShape self, SDWORD x, SDWORD y);
} vtable_Shape;

typedef struct _Shape {
	Object object;
	const vtable_Shape* vtable;
	DWORD color;
	DWORD border_color;
	Vertice position;
} Shape;

#define SHAPE(x) ((PShape)x)
#define SHAPE_VTABLE(x) SHAPE(x)->vtable

PShape Shape_constructor(PShape self);
void Shape_destructor(PObject self);

#endif /* _SHAPE_H_ */
