/**
 * \file container.h
 * \brief this file handles an abstraction for substructures in order to create containers (collections).
 */
#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <esic/esic.h>
#include <esic/object.h>

typedef struct _Container* PContainer;

/**
 * \struct _vtable_Container
 * \brief describes the vtable handled by subclasses of containers.
 */
typedef struct _vtable_Container {
	/*{@*/
	void (*pushBack)(PContainer, const PObject);	/**< method that appends an object to a collection */
	DWORD (*popBack)(PContainer, PObject);			/**< method that removes an object at the end of collection, and returns it */
	void (*pushFront)(PContainer, const PObject);	/**< method that adds an object at the beginning of the collection */
	DWORD (*popFront)(PContainer, PObject);			/**< method that removes an object at the beginning of the collection, and returns it */
	PObject (*at)(PContainer, DWORD);				/**< method that accesses a reference to an object given the index */
	void (*remove)(PContainer, PObject);			/**< method that removes the object provided in argument */
	void (*removeAt)(PContainer, DWORD);			/**< method that removes the object given its index */
	/*@}*/
} vtable_Container;

/**
 * \struct _Container
 * \brief describe a container, meaning a structure that will store objects, no matter how it manages to do so.
 */
typedef struct _Container {
	/*{@*/
	Object object;						/**< Base structure */
	const vtable_Container* vtable;		/**< Container's vtable */
	size_t count;						/**< logical number of elements */
	size_t unit_size;					/**< Size of any single element in the container */
	/*@}*/
} Container;

PContainer	Container_constructor(PContainer self, size_t unit_size);


void		Container_destructor(PObject self);
PObject		Container_clone(PObject self, PObject dst);
BOOL		Container_equalsTo(PObject self, PObject dst);
DWORD		Container_hash(PObject self);
DWORD		Container_type(PObject self);

#define CONTAINER(x)			((PContainer)x)
#define CONTAINER_VTABLE(x)		CONTAINER(x)->vtable

#endif /* _CONTAINER_H_ */
