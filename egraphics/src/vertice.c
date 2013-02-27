/**
 * \file vertice.c
 */
#include <esic/egraphics/vertice.h>

static const vtable_Object s_vtable_object = {
	Vertice_destructor,
	Vertice_clone,
	NULL,
	NULL
};

PVertice Vertice_constructor(PVertice self) {
	/* ... */
	self->object.size   = sizeof(Vertice);
	self->object.vtable = &s_vtable_object;
	self->x = self->y = 0;
	return self;
}

void Vertice_destructor(PObject self) {
	/* ... */
}

PObject Vertice_clone(PObject self, PObject dst) {
	PVertice real_self = (PVertice)self;
	PVertice real_dst  = (PVertice)dst;

	dst->size   = self->size;
	dst->vtable = self->vtable;

	real_dst->x = real_self->x;
	real_dst->y = real_self->y;

	return dst;
}