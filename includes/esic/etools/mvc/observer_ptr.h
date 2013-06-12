#ifndef _OBSERVER_PTR_H_
#define _OBSERVER_PTR_H_

#include <esic/etools/mvc/observer.h>

typedef struct _ObserverPtr* PObserverPtr;

typedef struct _ObserverPtr {
	Object object;
	PObserver observer;
	//BOOL dynamic; /* malloc or not? */
} ObserverPtr;

PObserverPtr ObserverPtr_constructor(PObserverPtr self, const PObserver observer/*, BOOL dynamic*/);

/* Virtual functions */
/* Object */
void ObserverPtr_destructor(PObject self);
PObject ObserverPtr_clone(PObject self, PObject dst);
BOOL ObserverPtr_equalsTo(PObject self, PObject dst);

#define OBSERVERPTR(x) ((PObserverPtr)x)

#endif /* _OBSERVER_PTR_H_ */