/**
 * \file event.c
 */
#include <esic/eapi/event.h>

static const char* event_strings[] = {
#define SYSTEM_EVENT(enum_value, string_value) string_value,
	SYSTEM_EVENT_LIST
#undef SYSTEM_EVENT
};

static const vtable_Object s_vtable_object = { Event_destructor, Event_clone, NULL, NULL };

PEvent Event_constructor(PEvent self) {
	self->object.vtable = &s_vtable_object;
	self->object.size   = sizeof(Event);

	return self;
}

void Event_destructor(PObject self) {

}

PObject Event_clone(PObject self, PObject dst) {
	PEvent real_self = (PEvent)self;
	PEvent real_dst  = (PEvent)dst;

	dst->size = self->size;
	dst->vtable = self->vtable;

	real_dst->type = real_self->type;
	real_dst->real_event = real_self->real_event;

	return dst;
}

const char* Event_enumToString(EventType event_type) {
	return event_strings[(DWORD)event_type];
}