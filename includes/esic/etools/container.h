/**
 * \file container.h
 * \brief this file handles an abstraction for substructures in order to create containers (collections).
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
	void (*remove)(PContainer, PObject);
	void (*removeAt)(PContainer, DWORD);
} vtable_Container;


typedef struct _Container {
	Object object;						/* Base structure */
	const vtable_Container* vtable;		/* Container's vtable */
	size_t count;						/* logical number of elements */
	size_t unit_size;					/* Size of any single element in the container */
} Container;

PContainer	Container_constructor(PContainer self, size_t unit_size);


void		Container_destructor(PObject self);
PObject		Container_clone(PObject self, PObject dst);
BOOL		Container_equalsTo(PObject self, PObject dst);
DWORD		Container_hash(PObject self);
DWORD		Container_type(PObject self);

/*
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type Container_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
*/

#define CONTAINER(x)			((PContainer)x)
#define CONTAINER_VTABLE(x)		CONTAINER(x)->vtable

#endif /* _CONTAINER_H_ */
