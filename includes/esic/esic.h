/**
 * \file esic.h
 * \brief provides the developer with architecture independent types, macros, and so on.
	It is one of the components that build the cornerstone of the esic framework.
 */
#ifndef _ESIC_H_
#define _ESIC_H_

#define ESIC_TYPES
#include <stdint.h>
typedef uint8_t BYTE;
typedef int8_t SBYTE;
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


/* Given the target, color order may change (until we can figure this out) */
#if defined(EMULATOR)

#define RGB_16B( r, g, b ) \
   ( ( ( ( (DWORD)(r) >> 3u ) & 0x1fu ) << 11u ) | \
     ( ( ( (DWORD)(g) >> 2u ) & 0x3fu ) <<  5u ) | \
     ( ( ( (DWORD)(b) >> 3u ) & 0x1fu ) <<  0u ) )
	  


#define RED(value)		((value >> 16) & 0xFF)
#define GREEN(value)	((value >> 8) & 0xFF)
#define BLUE(value) 	((value) & 0xFF)
	   
#elif defined (OPEN1788)

#define RGB_16B( r, g, b ) \
   ( ( ( ( (DWORD)(b) >> 3u ) & 0x1fu ) << 11u ) | \
     ( ( ( (DWORD)(g) >> 2u ) & 0x3fu ) <<  5u ) | \
     ( ( ( (DWORD)(r) >> 3u ) & 0x1fu ) <<  0u ) )
	  	   
#define BLUE(value)		((value >> 16) & 0xFF)
#define GREEN(value)	((value >> 8) & 0xFF)
#define RED(value) 	((value) & 0xFF)	   
#endif

#define CLASS(x) \
	typedef struct _##x x; \
	typedef struct _##x *P##x; \
	typedef struct _##x **PP##x; \
	typedef struct _##x

#define VTABLE(x) \
	const vtable_##x *vtable

#define SWAP(x,y) \
		x ^= y; \
		y ^= x; \
		x ^= y;

#endif
