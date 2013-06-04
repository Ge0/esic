/**
 * \file observer_ptr.c
 */
#include <esic/etools/mvc/observer_ptr.h>

static const vtable_Object s_vtable_object = {
	ObserverPtr_destructor,
	ObserverPtr_clone,
	NULL,
	NULL
};

PObserverPtr ObserverPtr_constructor(PObserverPtr self, const PObserver observer, BOOL dynamic) {

	/* Filling members */
	self->object.size = sizeof(ObserverPtr);
	self->observer = observer;
	self->dynamic = dynamic;

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	return self;

}

void ObserverPtr_destructor(PObject self) {
	/* Nothing do to */
}

PObject ObserverPtr_clone(PObject self, PObject dst) {
	PObserverPtr real_self = (PObserverPtr)self;
	PObserverPtr real_dst  = (PObserverPtr)dst;

	/* Copying members */
	dst->size        = self->size;
	dst->vtable      = self->vtable;
	
	real_dst->dynamic  = real_self->dynamic;
	real_dst->observer = real_self->observer;

	return dst;
}