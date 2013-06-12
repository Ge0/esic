#include <esic/etools/mvc/abstract_model.h>
#include <esic/etools/mvc/observer_ptr.h>

/*
void AbstractModel_addObserver(PAbstractModel self, const PObserver observer, BOOL dynamic) {
	ObserverPtr observer_ptr;

	ObserverPtr_constructor(&observer_ptr, observer, dynamic);
	
	List_pushBack(CONTAINER(&self->observers), OBJECT(&observer_ptr));

	ObserverPtr_destructor(OBJECT(&observer_ptr));

}

void AbstractModel_notify(PAbstractModel self, PObject data) {
	PListNode node = self->observers.head;

	while(node != NULL) {
		PObserver observer = (OBSERVERPTR(node->data))->observer;
		observer->vtable->update(observer, data);

		node = node->next;
	}
}
*/