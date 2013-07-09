/**
  * \file object.h
  * \brief this file handles Object structure definition & Object vtable.
  */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <esic/eapi/heap.h>
#include <esic/eapi/misc.h>
#include <esic/esic.h>
#include <stdio.h>

typedef struct _Object *PObject, **PPObject;

/**
  * \struct vtable_Object
  * \brief describes the vtable (like in C++) of an Object
  *
  */
typedef struct _vtable_Object {
	/*{@*/
	void	(*destructor)(PObject self);				/**<  destructor of an object (freeing the memory) */
	PObject (*clone)(PObject self, PObject dst);		/**<  cloning the object self into the dst memory space (previously allocted) */
	BOOL	(*equalsTo)(PObject self, PObject dst);		/**<  comparing two objects to test equality (TRUE or FALSE) */
	DWORD	(*hash)(PObject self);						/**<  hashing an object to store it into an hashtable */
	DWORD	(*type)(PObject self);						/**<  runtime identification */
	/*@}*/
} vtable_Object;

/**
  * \struct _Object
  * \brief describes the structure which almost every substructures inherit from.
  *
  * By inheriting this structure, sub-ones may access to advanced features such as being
  * stored into collections, being properly freeing (memory consumption), comparison, 
  * hashing, runtime type definition, and so on.
  */
typedef struct _Object {
	/*{@*/
	VTABLE(Object);			/**< Macro that actually references a const pointer to the vtable of an object */
	size_t size; 			/**< The size the object truly occupies in memory */
	/*@}*/
} Object;


#ifdef NEW
#undef NEW
#endif


/**
 * \def NEW
 * \brief this macro simulates the 'new' keyword in C++ and consists of allocating memory for our
 * structure and calling the constructor
 */
#define NEW(SELF, STRUCTURE) \
	SELF = (P##STRUCTURE)SicAlloc(sizeof(STRUCTURE)); \
	STRUCTURE##_constructor(SELF)

#ifdef DELETE
#undef DELETE
#endif

/**
 * \def DELETE
 * \brief this macro simulates the 'delete' keyword in C++ and consists of freed memory for our
 * structure and calling the destructor
 */
#define DELETE(SELF) \
	((PObject)SELF)->vtable->destructor((PObject)SELF); \
	SicFree(SELF); \
	SELF = NULL

/**
 * \def VTABLE_START
 * \brief this macro let the developer write the vtable definition for his structure in a kind of fancy format.
 */
#define VTABLE_START(base_class) \
	static const vtable_##base_class s_##base_class##_vtable =

/**
 * \def VTABLE_POINTER
 * \brief this macro let the developer retrieve a pointer to the vtable of the base class within the source code of his class.
 */
#define VTABLE_POINTER(base_class) &s_##base_class##_vtable

/* X-Macro */
/*
#define OBJECT_VIRTUAL_FUNCTIONS \
	OBJECT_VFUNCTION(void,		destructor,	"(PObject self)") \
	OBJECT_VFUNCTION(PObject,	clone,		"(PObject self, PObject dst)") \
	OBJECT_VFUNCTION(BOOL,		equalsTo,	"(PObject self, PObject dst)") \
	OBJECT_VFUNCTION(DWORD,		hash,		"(PObject self)") \
	OBJECT_VFUNCTION(DWORD,		type,		"(PObject self)")
*/

#define OBJECT(x)			((PObject)x)
#define OBJECT_VTABLE(x)	OBJECT(x)->vtable

/**
	destructor(self): should be called each time you need to free memory
	clone(self, dst): will clone self into dst. do *NOT* construct dst, it's already done for you BUT you have to destruct dst then
	equalsTo(self,dst): will perform a customized comparison between self & dst
	hash(self): will generate a DWORD which should basically identify the values in an unique way.
*/

#endif /* _OBJECT_H_ */
