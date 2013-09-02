/**
 * \file heap.h
 */
#ifndef _HEAP_H_
#define _HEAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <esic/esic.h>

/**
  * \fn void* Debug_SicAlloc(size_t size);
  * \brief malloc() wrapping: allocs a size'd chunck into the heap, 
  * performs an internal record of its pointer & returns it
  * \param size the size of the memory chunk to allocate
  * \return void* pointer to the allocated memory chunk
  */
void* Debug_SicAlloc(size_t size);

/**
  * \fn void Debug_SicFree(void* ptr);
  * \brief frees a memory chunk that has previously been allocated with the Debug_SicAlloc() function,
  * by unregistering the chunk as well
  * \void ptr pointer to the allocated chunk to free
  * \return void
  */
void Debug_SicFree(void* ptr);

/**
  * \fn char* SicStrdup(const char* str);
  * \brief this function acts as strdup(), exept the fact that is does call Debug_SicAlloc() instead of malloc()
  * \param str string tu duplicate
  * \return char* pointer to the duplicated string (has to be Debug_SicFree()'d then)
  */
char* SicStrdup(const char* str);

/**
  * \fn void SicHeapDump();
  * \brief dumps the allocated memory chunk into the debug output
  * \return void
  */
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


/**
  * \def SicAssert(expr)
  * \brief ensures that expr is actually a valid expression, otherwise outputs onto debug stream
  * informations about failed assertion.
  */
#define SicAssert(expr) \
if(!(expr))  {\
	SicPrintfDebug("Assertion failed at %s:%d\r\n", \
		__FILE__, __LINE__); \
	while(1); \
	}


#endif /* _HEAP_H_ */
