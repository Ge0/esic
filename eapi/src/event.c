#include <esic/eapi/event.h>

static const vtable_Object s_vtable_object = { Event_destructor, NULL, NULL, NULL };

PEvent Event_constructor(PEvent self) {
	self->object.vtable = &s_vtable_object;
	self->object.size   = sizeof(Event);

	return self;
}

void Event_destructor(PObject self) {

}