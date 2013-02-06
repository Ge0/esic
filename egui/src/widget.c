#include <esic/egui/widget.h>
#include <esic/eapi/event.h>

static const vtable_Object s_vtable_object = { NULL, NULL, NULL, NULL };

PWidget Widget_constructor(PWidget self, PWidget parent) {
	self->object.size = sizeof(Widget);


	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	/* Filling members */
	self->parent = parent;

	return self;
}

void Widget_defaultProc(PWidget self, PEvent systemEvent) {

}

