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




#endif /* _HEAP_H_ */
