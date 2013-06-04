#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <esic/object.h>
#include <esic/etools/mvc/abstract_model.h>

typedef struct _Observer* PObserver;

typedef struct _vtable_Observer {
	void (*update)(PObserver, PObject);
} vtable_Observer;

typedef struct _Observer {
	Object object;
	VTABLE(Observer);
	PAbstractModel model;
} Observer;

/* Test virtual inheritance */
typedef struct _VObserver {
	PObject object;			/* Becomes a pointer */
	VTABLE(Observer);
	PAbstractModel model;
} VObserver, *PVObserver;

PObserver Observer_constructor(PObserver self);

#endif /* _OBSERVER_H_ */