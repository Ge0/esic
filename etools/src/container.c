#include <esic/etools/container.h>

PContainer Container_constructor(PContainer self, size_t unit_size) {
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* Filling object's vtable */
	/*
	self->object.vtable.destructor = Container_destructor;
	*/

	self->object.vtable = &s_container_object_vtable;

	/* There are not any element in the container */
	self->unit_size = unit_size;
	self->count     = 0;

	return self;
}

void Container_destructor(PObject self) {
}