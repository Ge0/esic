/**
 * \file shape_ptr.h
 */
#ifndef _SHAPE_PTR_H_
#define _SHAPE_PTR_H_

#include <esic/egraphics/shape.h>


/* In order to store pointers into containers such as Vector, List, and so on. */
typedef struct _ShapePtr {
	Object object;
	PShape shape;
	BOOL dynamic; /* malloc or not? */
} ShapePtr, *PShapePtr;

PShapePtr ShapePtr_constructor(PShapePtr self, PShape Shape, BOOL dynamic);

/* Virtual functions */
/* Object */
void ShapePtr_destructor(PObject self);
PObject ShapePtr_clone(PObject self, PObject dst);

#define SHAPEPTR(x) ((PShapePtr)x)

#endif /* _SHAPE_PTR_H_ */