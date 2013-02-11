#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <assert.h>
#include <esic/etools/container.h>

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
PObject Vector_clone(PObject self, PObject dst);
BOOL Vector_equalsTo(PObject self, PObject dst);
DWORD Vector_hash(PObject self);

/* Container */
void Vector_pushBack(PContainer self, const PObject data);
DWORD Vector_popBack(PContainer self, PObject popped);
void Vector_pushFront(PContainer self, const PObject data);
DWORD Vector_popFront(PContainer self, PObject popped);
PObject Vector_at(PContainer self, DWORD index);

/* End of virtual functions */

#endif /* _VECTOR_H_ */