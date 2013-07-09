/**
 * \file shape.h
 */
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <esic/object.h>
#include <esic/egraphics/painter.h>

typedef struct _Shape *PShape;

typedef struct _vtable_Shape {
	void (*paint)(PShape self, PPainter painter);
} vtable_Shape;

typedef struct _Shape {
	Object object;
	const vtable_Shape* vtable;
	DWORD color;
	DWORD border_color;
} Shape;

#define SHAPE(x) ((PShape)x)

#endif /* _SHAPE_H_ */
