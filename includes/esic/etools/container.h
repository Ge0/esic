/**
 * container.h
 */
#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <esic/esic.h>
#include <esic/object.h>

typedef struct _Container* PContainer;

typedef struct _vtable_Container {
	void (*pushBack)(PContainer, const PObject);
	DWORD (*popBack)(PContainer, PObject);
	void (*pushFront)(PContainer, const PObject);
	DWORD (*popFront)(PContainer, PObject);
	PObject (*at)(PContainer, DWORD);
} vtable_Container;


typedef struct _Container {
	Object object;						/* Base structure */
	const vtable_Container* vtable;		/* Container's vtable */
	size_t count;						/* logical number of elements */
	size_t unit_size;					/* Size of any single element in the container */
} Container;

PContainer	Container_constructor(PContainer self, size_t unit_size);

/*
void		Container_destructor(PObject self);
PObject		Container_clone(PObject self, PObject dst);
BOOL		Container_equalsTo(PObject self, PObject dst);
DWORD		Container_hash(PObject self);
*/

#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Container_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

/* X-Macro */
#define CONTAINER_VIRTUAL_FUNCTIONS \
	CONTAINER_VFUNCTION(void,		pushBack,	(PContainer self, const PObject)) \
	CONTAINER_VFUNCTION(DWORD,		popBack,	(PContainer self, PObject popped)) \
	CONTAINER_VFUNCTION(void,		pushFront,	(PContainer self, PObject dst)) \
	CONTAINER_VFUNCTION(DWORD,		popFront,	(PContainer self, Pobject popped)) \
	CONTAINER_VFUNCTION(PObject,	at,			(PContainer self, DWORD index))


#define CONTAINER(x)			((PContainer)x)
#define CONTAINER_VTABLE(x)		CONTAINER(x)->vtable

#endif /* _CONTAINER_H_ */