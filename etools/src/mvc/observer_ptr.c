/**
 * \file observer_ptr.c
 */
#include <esic/etools/mvc/observer_ptr.h>

static const vtable_Object s_vtable_object = {
	ObserverPtr_destructor,
	ObserverPtr_clone,
	ObserverPtr_equalsTo,
	NULL,
	NULL
};

PObserverPtr ObserverPtr_constructor(PObserverPtr self, const PObserver observer) {

	/* Filling members */
	self->object.size = sizeof(ObserverPtr);
	self->observer = observer;
	//self->dynamic = dynamic;

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
	
	//real_dst->dynamic  = real_self->dynamic;
	real_dst->observer = real_self->observer;

	return dst;
}

BOOL ObserverPtr_equalsTo(PObject self, PObject dst) {
	return (BOOL)(OBSERVERPTR(self)->observer == OBSERVERPTR(dst)->observer);
}

DWORD ObserverPtr_hash(PObject self) {
	/* TODO. */

	return 0;
}

DWORD ObserverPtr_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("ObserverPtr");
	}

	return s_hash;
}