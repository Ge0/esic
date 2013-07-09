/**
 * \file vector.h
 * \brief this file describes a vector structure; vectors aim to store objects like the well known std::vector class in C++.
 */
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <esic/etools/container.h>

#define INITIAL_SIZE	5

typedef struct _Vector* PVector;

/**
  * \struct _Vector
  * \brief the vector structure handles stuff to store data accessible by an index.
  */
typedef struct _Vector {
	/*@{*/
	Container	container;	/**< inherits from container */
	size_t		allocated_size; /**< the total allocated size of a vector (private usage) */
	void*		elements;	/**< pointer to the allocated memory space (private usage) */
	/*@}*/
} Vector;

/* Member functions */
/**
  * \fn PVector Vector_constructor(PVector self, size_t unit_size);
  * \brief constructs a Vector instance.
  * \param self pointer the 'self' instance
  * \param unit_size unit size of a stored object into the vector
  * \return pointer to the self instance
  */
PVector Vector_constructor(PVector self, size_t unit_size);


/* Virtual functions */

/* Object */

/**
  * \fn void Vector_destructor(PObject self);
  * \brief destructs a Vector instance and free its allocated memory
  * \param self the self instance casted to PObject
  * \return void
  */
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
void Vector_remove(PContainer self, PObject object);
void Vector_removeAt(PContainer self, DWORD index);
/* End of virtual functions */

#define VECTOR(x) ((PVector)x)

#endif /* _VECTOR_H_ */
