/**
 * szstring.h
 */
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
void SzString_append(PSzString self, char ch);
void SzString_removeLastChar(PSzString self);
void SzString_insertCharAt(PSzString self, DWORD pos, char ch);
void SzString_subString(PSzString self, DWORD start, DWORD n, PSzString out);
void SzString_removeCharAt(PSzString self, DWORD pos);


#endif /* _SZSTRING_H_ */