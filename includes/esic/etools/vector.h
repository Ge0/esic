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
  * \brief the vector structure handles stuff to store accessible data by indexes.
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

/**
  * \fn void Vector_clone(PObject self, PObject dst);
  * \brief clones the vector self into another one dst
  * \param self the self instance casted to PObject
  * \param dst the destination instance casted to PObject, which has to point into a large enough buffer
  * \return PObject: pointer to the dst instance
  */
PObject Vector_clone(PObject self, PObject dst);

/**
  * \fn void Vector_equalsTo(PObject self, PObject dst);
  * \brief compares the self vector to the dst vector, by doing an element-whise comparison
  * \param self the self instance casted to PObject
  * \param dst the destination instance casted to PObject
  * \return BOOL: TRUE if vectors contain the same elements, FALSE otherwise
  */
BOOL Vector_equalsTo(PObject self, PObject dst);


/**
  * \fn DWORD Vector_hash(PObject self);
  * \brief return an unique hash that identifies the vector; useful to store it into an hashtable
  * \param self the self instance casted to PObject
  */
DWORD Vector_hash(PObject self);


/* Container */

/**
  * \fn void Vector_pushBack(PContainer self, const PObject data);
  * \brief pushes the data element at the end of the self container
  * \param self the instance casted to PContainer
  * \param data the instance casted to PObject to append into the container
  * \return void
  */
void Vector_pushBack(PContainer self, const PObject data);

/**
  * \fn DWORD Vector_popBack(PContainer self, PObject popped);
  * \brief pops into popped the last element at the end of the container self
  * \param self the instance casted to PContainer
  * \param popped the instance casted to PObject that will handle the popped data, which has to point into a large enough buffer
  * \return DWORD: 1 if the popping succeeded, 0 otherwise
  */
DWORD Vector_popBack(PContainer self, PObject popped);

/**
  * \fn void Vector_pushFront(PContainer self, const PObject data);
  * \brief pushes the data element at the beginning of the self container
  * \param self the instance casted to PContainer
  * \param data the instance casted to PObject to append into the container
  * \return void
  */
void Vector_pushFront(PContainer self, const PObject data);

/**
  * \fn DWORD Vector_popFront(PContainer self, PObject popped);
  * \brief pops into popped the first element at the beginning of the self container
  * \param self the instance casted to PContainer
  * \param popped the instance casted to PObject that will handle the popped data, which has to point into a large enough buffer
  * \return DWORD: 1 if the popping succeeded, 0 otherwise
  */ 
DWORD Vector_popFront(PContainer self, PObject popped);

/**
  * \fn PObject Vector_at(PContainer self, DWORD index);
  * \brief Accesses the index-th element into the self container
  * \param self the self instance casted to PContainer
  * \param index the index of the instance to access into the container, from zero to size-1
  * \return PObject pointer to the accessed instance - not a copy - if the index fits into the size of the container, NULL otherwise
  */
PObject Vector_at(PContainer self, DWORD index);

/**
  * \fn void Vector_remove(PContainer self, PObject object);
  * \brief removes the object into the self container
  * \param self the self container casted to PContainer
  * \param object the object to remove
  * \return void
  */
void Vector_remove(PContainer self, PObject object);

/**
  * \fn void Vector_removeAt(PContainer self, PObject index);
  * \brief removes the object at index into the self container
  * \param self the self instance casted to PContainer
  * \param index the index of the instance to remove from the container, from zero to size-1
  * \return void
  */
void Vector_removeAt(PContainer self, DWORD index);
/* End of virtual functions */

/**
  * \def VECTOR
  * \brief fancy casting for an instance to PVector
  */
#define VECTOR(x) ((PVector)x)

#endif /* _VECTOR_H_ */
