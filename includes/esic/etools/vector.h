#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <assert.h>
#include "container.h"

#define INITIAL_SIZE	5

typedef struct _Vector* PVector;

typedef struct _Vector {
	Container	container;
	size_t		allocated_size;
	void*		elements;
} Vector;

/* Member functions */
PVector Vector_constructor(PVector self, size_t unit_size);


/* Virtual functions */

/* Object */
void Vector_destructor(PObject self);

/* Container */
void Vector_pushBack(PContainer self, const PObject data);
DWORD Vector_popBack(PContainer self, PObject popped);
PObject Vector_at(PContainer self, DWORD index);

/* End of virtual functions */

/* Vtables definition */
static const vtable_Object s_vector_object_vtable       = { Vector_destructor, NULL, NULL, NULL };
static const vtable_Container s_vector_container_vtable = { Vector_pushBack, Vector_popBack, Vector_at };

#endif /* _VECTOR_H_ */