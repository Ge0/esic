/**
  * \file object.h
  * \brief this file handles Object structure definition & Object vtable.
  */
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <esic/eapi/heap.h>
#include <esic/esic.h>
#include <stdio.h>

typedef struct _Object *PObject, **PPObject;

typedef struct _vtable_Object {
	void	(*destructor)(PObject self);
	PObject (*clone)(PObject self, PObject dst);
	BOOL	(*equalsTo)(PObject self, PObject dst);
	DWORD	(*hash)(PObject self);
} vtable_Object;

/**
  * \struct Object
  * \brief describes the structure which almost every substructures inherit from.
  *
  * By inheriting this structure, sub-ones may access to advanced features such as being
  * stored into collections, being properly freeing (memory consumption), comparison, 
  * hashing, and so on.
  */
CLASS(Object) {
	VTABLE(Object);
	size_t size; 
};

#ifdef NEW
#undef NEW
#endif
#define NEW(SELF, STRUCTURE) \
	SELF = (P##STRUCTURE)SicAlloc(sizeof(STRUCTURE)); \
	STRUCTURE##_constructor(SELF)

#ifdef NEW
#undef DELETE
#endif
#define DELETE(SELF) \
	((PObject)SELF)->vtable->destructor((PObject)SELF); \
	SicFree(SELF); \
	SELF = NULL


#define VTABLE_START(base_class) \
	static const vtable_##base_class s_##base_class##_vtable =

#define VTABLE_POINTER(base_class) &s_##base_class##_vtable

/* X-Macro */
#define OBJECT_VIRTUAL_FUNCTIONS \
	OBJECT_VFUNCTION(void,		destructor, (PObject self)) \
	OBJECT_VFUNCTION(PObject,	clone,		(PObject self, PObject dst)) \
	OBJECT_VFUNCTION(BOOL,		equalsTo,	(PObject self, PObject dst)) \
	OBJECT_VFUNCTION(DWORD,		hash,		(PObject self))


#define OBJECT(x)			((PObject)x)
#define OBJECT_VTABLE(x)	OBJECT(x)->vtable

/**
	destructor(self): should be called each time you need to free memory
	clone(self, dst): will clone self into dst. do *NOT* construct dst, it's already done for you BUT you have to destruct dst then
	equalsTo(self,dst): will perform a customized comparison between self & dst
	hash(self): will generate a DWORD which should basically identify the values in an unique way.
*/

#endif /* _OBJECT_H_ */
