#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <esic/object.h>

typedef struct _Observer* PObserver;

typedef struct _vtable_Observer {
	void (*update)(PObserver, PObject);
} vtable_Observer;

typedef struct _Observer {
	Object object;
	VTABLE(Observer);
} Observer;

#endif /* _OBSERVER_H_ */