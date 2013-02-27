/**
 * container.c
 */
#include <esic/etools/container.h>

static const vtable_Object s_object_vtable = {
	Container_destructor,
	NULL,
	NULL,
	NULL
};

PContainer Container_constructor(PContainer self, size_t unit_size) {
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* Filling object's vtable */
	/*
	self->object.vtable.destructor = Container_destructor;
	*/

	self->object.vtable = &s_object_vtable;

	/* There are not any element in the container */
	self->unit_size = unit_size;
	self->count     = 0;

	return self;
}

void Container_destructor(PObject self) {
}