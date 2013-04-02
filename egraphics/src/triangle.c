/**
 * \file triangle.h
 */
#include <esic/egraphics/triangle.h>

static const vtable_Object s_object_vtable = {
	Triangle_destructor,
	Triangle_clone,
	NULL,
	NULL
};

static const vtable_Shape s_shape_vtable = {
	Triangle_paint
};

PTriangle Triangle_constructor(PTriangle self) {
	self->shape.object.size = sizeof(Triangle);
	self->shape.object.vtable = &s_object_vtable;
	self->shape.vtable = &s_shape_vtable;

	return self;
}

void Triangle_destructor(PObject self) {

}

PObject Triangle_clone(PObject self, PObject dst) {
	PTriangle real_self = (PTriangle) self;
	PTriangle real_dst  = (PTriangle) dst;

	dst->vtable = self->vtable;
	dst->size   = self->size;

	real_dst->v1 = real_self->v1;
	real_dst->v2 = real_self->v2;
	real_dst->v3 = real_self->v3;


	return dst;

}

BOOL Triangle_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Triangle_hash(PObject self) {
	/* TODO. */
	return 0;
}

void Triangle_paint(PShape self, PAbstractPainter painter) {

}