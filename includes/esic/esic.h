#ifndef _ESIC_H_
#define _ESIC_H_

#ifndef _WIN32

	#define ESIC_TYPES
	#include <stdint.h>
	typedef uint8_t BYTE;
	typedef int16_t SWORD;
	typedef uint16_t  WORD;
	typedef int32_t SDWORD;
	typedef uint32_t DWORD;
	typedef uint64_t QWORD;
	typedef int64_t SQWORD;

	typedef uint8_t UCHAR;
	typedef signed long int LONG;
	typedef unsigned long int ULONG;
	typedef unsigned short	WCHAR;
	
	/* These types must be 16-bit, 32-bit or larger integer */
	typedef int             INT;
	typedef unsigned int	UINT;

	#define ESIC_MACROS
	#define HIWORD(value) ((value >> 16) & 0xFFFF)
	#define LOWORD(value) (value & 0xFFFF)
	
	typedef enum _BOOL {
	  FALSE = 0,
	  TRUE
	} BOOL;
	

#else
/*
#define ESIC_TYPES
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint8_t BOOL;
typedef int16_t SWORD;
typedef uint16_t  WORD;
typedef int32_t SDWORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef int64_t SQWORD;

#ifndef VOID
#define VOID void
#endif

//typedef void VOID;
typedef uint8_t UCHAR;
typedef signed long int LONG;
typedef unsigned long int ULONG;
typedef unsigned short	WCHAR;
	
// These types must be 16-bit, 32-bit or larger integer
typedef int             INT;
typedef unsigned int	UINT;

#define ESIC_MACROS
#define HIWORD(value) ((value >> 16) & 0xFFFF)
#define LOWORD(value) (value & 0xFFFF)
*/
/*
typedef enum _BOOL {
	FALSE = 0,
	TRUE
} BOOL;
*/
/*
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
*/


#include <Windows.h>
#include <stdint.h>
	
typedef int16_t SWORD;
typedef int32_t SDWORD;
#endif
	


	

#define RGB_16B( r, g, b ) \
   ( ( ( ( (DWORD)(r) >> 3u ) & 0x1fu ) << 11u ) | \
     ( ( ( (DWORD)(g) >> 2u ) & 0x3fu ) <<  5u ) | \
     ( ( ( (DWORD)(b) >> 3u ) & 0x1fu ) <<  0u ) )

#define RED(value)		((value >> 16) & 0xFF)
#define GREEN(value)	((value >> 8) & 0xFF)
#define BLUE(value) 	((value) & 0xFF)

#define CLASS(x) \
	typedef struct _##x x; \
	typedef struct _##x *P##x; \
	typedef struct _##x **PP##x; \
	typedef struct _##x

#define VTABLE(x) \
	const vtable_##x *vtable

#if defined EMULATOR
#if defined (WIN32)
ULONG
_cdecl
DbgPrint(
    PCH Format,
    ...
    );
#define SicPrintfDebug	DbgPrint


#endif /* WIN32 */
#endif /* EMULATOR */

#endif
