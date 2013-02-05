#ifndef _SZSTRING_H_
#define _SZSTRING_H_


#include <esic/object.h>


typedef struct _SzString {
	Object object;
	size_t size;
	char* data;
} SzString, *PSzString;

PSzString SzString_constructor(PSzString self, const char* data);

/* Virtual functions */

/* Object */
void	SzString_destructor(PObject self);
PObject SzString_clone(PObject self, PObject dst);
BOOL	SzString_equalsTo(PObject self, PObject dst);
DWORD	SzString_hash(PObject self);

/* Methods */
void SzString_setData(PSzString self, const char* data);

/* Vtables definition */
static const vtable_Object s_szstring_object_vtable  = { SzString_destructor, SzString_clone, SzString_equalsTo, SzString_hash };



#endif /* _SZSTRING_H_ */