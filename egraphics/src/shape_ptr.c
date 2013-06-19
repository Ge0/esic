/**
 * \file widget_ptr.c
 */
#include <esic/egraphics/shape_ptr.h>

static const vtable_Object s_vtable_object = {
	ShapePtr_destructor,
	ShapePtr_clone,
	NULL,
	NULL
};

PShapePtr ShapePtr_constructor(PShapePtr self, PShape shape, BOOL dynamic) {

	/* Filling members */
	self->object.size = sizeof(ShapePtr);
	self->shape = shape;
	self->dynamic = dynamic;

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	return self;

}

void ShapePtr_destructor(PObject self) {
	/* Nothing do to */
}

PObject ShapePtr_clone(PObject self, PObject dst) {
	PShapePtr real_self = (PShapePtr)self;
	PShapePtr real_dst  = (PShapePtr)dst;

	/* Copying members */
	dst->size        = self->size;
	dst->vtable      = self->vtable;
	
	real_dst->dynamic = real_self->dynamic;
	real_dst->shape = real_self->shape;

	return dst;
}