/**
 * hashnode.h
 */
#ifndef _HASHNODE_H_
#define _HASHNODE_H_

#include <esic/object.h>

typedef struct _Hashnode {
	Object object;
	PObject key;
	PObject value;
	struct _Hashnode* next;
} Hashnode, *PHashnode, **PPHashnode;


/*
#define HASHNODE_BY_KEY	sizeof(Object)
#define HASHNODE_BY_VALUE sizeof(Object) + sizeof(PObject)
*/

/*
#define HASHNODE_BY_KEY (DWORD)offsetof(Hashnode, key)
#define HASHNODE_BY_VALUE (DWORD)offsetof(Hasnode, value)
*/


#define HASHNODE_BY_KEY ((DWORD)(&((PHashnode)0)->key))
#define HASHNODE_BY_VALUE ((DWORD)(&((PHashnode)0)->value))


PHashnode Hashnode_constructor(PHashnode self);

/* Virtual functions */

/* Object */
void	Hashnode_destructor(PObject self);
PObject Hashnode_clone(PObject self, PObject dst);
BOOL	Hashnode_equalsTo(PObject self, PObject dst);
DWORD	Hashnode_hash(PObject self);

void Hashnode_setKey(PHashnode self, PObject key);
void Hashnode_setValue(PHashnode self, PObject value);
void Hashnode_setNext(PHashnode self, PHashnode next);

/* Vtables definition */
static const vtable_Object s_hashnode_object_vtable = { Hashnode_destructor, Hashnode_clone, Hashnode_equalsTo, Hashnode_hash };


#endif /* _HASHNODE_H_ */
