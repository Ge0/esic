/**
 * \file heap.h
 */
#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <esic/esic.h>

void* Debug_SicAlloc(size_t size);
void Debug_SicFree(void* ptr);
char* SicStrdup(const char* str);
void SicHeapDump();
void SicHeapClean();

#if defined (linux)
ULONG LinuxSicPrintfDebug(const char* format, ...);
#endif

#if defined EMULATOR
#if defined (WIN32)
extern ULONG
_cdecl
DbgPrint(
    char* format,
    ...
    );
#define SicPrintfDebug	DbgPrint

#elif defined (linux)

#define SicPrintfDebug LinuxSicPrintfDebug

#endif /* WIN32/linux */

#elif defined(OPEN1788)
extern void xprintf (const char* fmt, ...);
#define SicPrintfDebug xprintf

#endif /* EMULATOR */




#ifndef _DEBUG 
#define SicAlloc(x) malloc(x)
#define SicFree(x)  free(x)
#else
#define SicAlloc(x) Debug_SicAlloc(x)
#define SicFree(x)  Debug_SicFree(x)
#endif /* _DEBUG */


typedef struct _link {
	void* ptr;
	size_t memory_space;
	struct _link* next;
} link, *plink;


#define SicAssert(expr) \
if(!(expr))  {\
	SicPrintfDebug("Assertion failed at %s:%d\r\n", \
		__FILE__, __LINE__); \
	while(1); \
	}


#endif /* _HEAP_H_ */
