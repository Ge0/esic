/**
 * \file widget_ptr.c
 */
#include <esic/egui/widget_ptr.h>

static const vtable_Object s_vtable_object = {
	WidgetPtr_destructor,
	WidgetPtr_clone,
	NULL,
	NULL
};

PWidgetPtr WidgetPtr_constructor(PWidgetPtr self, PWidget widget) {

	/* Filling members */
	self->object.size = sizeof(WidgetPtr);
	self->widget = widget;

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	return self;

}

void WidgetPtr_destructor(PObject self) {
	/* Nothing do to */
}

PObject WidgetPtr_clone(PObject self, PObject dst) {
	PWidgetPtr real_self = (PWidgetPtr)self;
	PWidgetPtr real_dst  = (PWidgetPtr)dst;

	/* Copying members */
	dst->size        = self->size;
	dst->vtable      = self->vtable;
	real_dst->widget = real_self->widget;

	return dst;
}