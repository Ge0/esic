#ifndef _ZSTRING_BUFFER_H_
#define _ZSTRING_BUFFER_H_

#include <esic/object.h>

#define ZSTRINGBUFFER_INITIAL_SIZE	15

typedef struct _ZStringBuffer *PZStringBuffer;

typedef struct _ZStringBuffer {
	Object object;
	size_t logical_size;
	size_t buffer_size;
	char* data;
} ZStringBuffer;

PZStringBuffer ZStringBuffer_constructor(PZStringBuffer self);
PZStringBuffer ZStringBuffer_append(PZStringBuffer self, const char* data);
void ZStringBuffer_subString(PZStringBuffer self, DWORD start, DWORD n, PZStringBuffer out);
void ZStringBuffer_setData(PZStringBuffer self, const char* data);
void ZStringBuffer_removeLastChar(PZStringBuffer self);
void ZStringBuffer_insertCharAt(PZStringBuffer self, DWORD pos, char ch);
void ZStringBuffer_removeCharAt(PZStringBuffer self, DWORD pos);


#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type ZStringBuffer_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

#define ZSTRINGBUFFER(x) ((PZStringBuffer)x)

#endif /* _ZSTRING_BUFFER_H_ */