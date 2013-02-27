/**
 * hashtable.h
 */
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stddef.h>

#define HASHTABLE_SIZE 10

#include <esic/etools/hashnode.h>
#include <esic/etools/map.h>

typedef struct _Hashtable {
	Map	map;
	size_t allocated_size;
	PPHashnode nodes;
} Hashtable, *PHashtable;


PHashtable Hashtable_constructor(PHashtable self, size_t size, size_t unit_size_key, size_t unit_size_value);
DWORD Hashtable_hashString(const char* string);

/* Virtual functions */

/* Object */
void	Hashtable_destructor(PObject self);
PObject Hashtable_clone(PObject self, PObject dst);
BOOL	Hashtable_equalsTo(PObject self, PObject dst);

/* Map */
void Hashtable_clear(PMap self);
BOOL Hashtable_containsKey(PMap self, PObject key);
BOOL Hashtable_containsValue(PMap self, PObject value);
PObject Hashtable_get(PMap self, PObject key);
void Hashtable_put(PMap self, PObject key, PObject value);

/* End of virtual functions */

/* Vtables definition */
static const vtable_Object s_hashtable_object_vtable  = { Hashtable_destructor, Hashtable_clone, Hashtable_equalsTo, NULL };
static const vfptr_Map s_hashtable_map_vtable  = { Hashtable_clear, Hashtable_containsKey, Hashtable_containsValue, Hashtable_get, Hashtable_put };

#endif /* _HASHTABLE_H_ */
