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



/* vtable declaration */
/*
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Shape_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
*/

/* X-Macro */
#define SHAPE_VIRTUAL_FUNCTIONS \
	SHAPE_VFUNCTION(void, paint, (PShape self, PAbstractPainter painter))


#define SHAPE(x) ((PShape)x)

#endif /* _SHAPE_H_ */
