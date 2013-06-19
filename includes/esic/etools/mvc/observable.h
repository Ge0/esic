#ifndef _OBSERVABLE_H_
#define _OBSERVABLE_H_

#include <esic/object.h>
#include <esic/etools/list.h>

// Forward declaration
typedef struct _ObserverPtr* PObserverPtr;
typedef struct _Observer* PObserver;

typedef struct _Observable* PObservable;

typedef struct _Observable {
	Object object;
	List observers;		/* List of ObserverPtr */
	BOOL changed;
} Observable;

PObservable Observable_constructor(PObservable self);
void Observable_addObserver(PObservable self, const PObserver observer);
void Observable_removeObserver(PObservable self, const PObserver observer);
void Observable_notify(PObservable self, PObject data);

/* Virtual functions */
/* Object */
void Observable_destructor(PObject self);
PObject Observable_clone(PObject self, PObject dst);
BOOL Observable_equalsTo(PObject self, PObject dst);
DWORD Observable_hash(PObject self);
DWORD Observable_type(PObject self);

#define OBSERVABLE(x) ((PObservable)x)

#endif /* _OBSERVABLE_H_ */