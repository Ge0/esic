/**
 * \file abstract_view_ptr.c
 */
#include <esic/object.h>

static const vtable_Object s_vtable_object = {
	AbstractViewPtr_destructor,
	AbstractViewPtr_clone,
	NULL,
	NULL
};

PAbstractViewPtr AbstractViewPtr_constructor(PAbstractViewPtr self, PAbstractView abstract_view, BOOL dynamic) {

	/* Filling members */
	self->object.size = sizeof(AbstractViewPtr);
	self->abstract_view = abstract_view;
	self->dynamic = dynamic;

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	return self;

}

void AbstractViewPtr_destructor(PObject self) {
	/* Nothing do to */
}

PObject WidgetPtr_clone(PObject self, PObject dst) {
	PAbstractViewPtr real_self = (PAbstractViewPtr)self;
	PAbstractViewPtr real_dst  = (PAbstractViewPtr)dst;

	/* Copying members */
	dst->size        = self->size;
	dst->vtable      = self->vtable;
	
	real_dst->dynamic       = real_self->dynamic;
	real_dst->abstract_view = real_self->abstract_view;

	return dst;
}