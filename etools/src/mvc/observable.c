#include <esic/etools/mvc/observable.h>
#include <esic/etools/mvc/observer_ptr.h>

VTABLE_START(Object) {
	Observable_destructor,
	Observable_clone,
	Observable_equalsTo,
	Observable_hash,
	Observable_type
};

PObservable Observable_constructor(PObservable self) {
	
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	OBJECT(self)->size   = sizeof(Observable);

	List_constructor(&self->observers, sizeof(ObserverPtr));

	self->changed = FALSE;

	return self;
}
void Observable_addObserver(PObservable self, const PObserver observer) {
	ObserverPtr observer_ptr;

	ObserverPtr_constructor(&observer_ptr, observer/*, dynamic */);

	List_pushBack(CONTAINER(&self->observers), OBJECT(&observer_ptr));

	/* update the observer then */
	observer->vtable->update(observer, NULL);
}

void Observable_notify(PObservable self, PObject data) {
	PListNode node = self->observers.head;

	while(node != NULL) {
		PObserver observer = (OBSERVERPTR(node->data))->observer;
		observer->vtable->update(observer, data);

		node = node->next;
	}
}

void Observable_removeObserver(PObservable self, const PObserver observer) {
	CONTAINER_VTABLE(&self->observers)->remove(CONTAINER(&self->observers), OBJECT(observer));
}

void Observable_destructor(PObject self) {
	List_destructor(OBJECT(&OBSERVABLE(self)->observers));
}

PObject Observable_clone(PObject self, PObject dst) {
	/* TODO. */

	return dst;
}

BOOL Observable_equalsTo(PObject self, PObject dst) {
	/* TODO. */

	return FALSE;
}
DWORD Observable_hash(PObject self) {
	/* TODO. */

	return 0;
}

DWORD Observable_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Observable");
	}
	return s_hash;
}