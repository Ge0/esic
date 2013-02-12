#ifndef _SHAPE_H_
#define _SHAPE_H_

typedef struct _Shape *PShape;

typedef struct _vtable_Shape {
	void (*paint)(PShape);
} vtable_Shape;

typedef struct _Shape {
	Object object;
	const vtable_Shape* vtable;
} Shape, *PShape;

#endif /* _SHAPE_H_ */