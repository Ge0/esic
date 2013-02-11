#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <esic/e11_types.h>
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
void		Container_destructor(PObject self);




#endif /* _CONTAINER_H_ */