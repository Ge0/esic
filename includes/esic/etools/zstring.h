/**
 * ZString.h
 */
#ifndef _ZString_H_
#define _ZString_H_


#include <esic/object.h>


typedef struct _ZString {
	Object object;
	size_t size;
	char* data;
} ZString, *PZString;

PZString ZString_constructor(PZString self, const char* data);

/* Virtual functions */

/* Object */
/*
void	ZString_destructor(PObject self);
PObject ZString_clone(PObject self, PObject dst);
BOOL	ZString_equalsTo(PObject self, PObject dst);
DWORD	ZString_hash(PObject self);
*/
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type ZString_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

/* Methods */
void ZString_setData(PZString self, const char* data);
void ZString_append(PZString self, char ch);
void ZString_removeLastChar(PZString self);
void ZString_insertCharAt(PZString self, DWORD pos, char ch);
void ZString_subString(PZString self, DWORD start, DWORD n, PZString out);
void ZString_removeCharAt(PZString self, DWORD pos);


#endif /* _ZString_H_ */