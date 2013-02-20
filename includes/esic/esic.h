#ifndef _ESIC_H_
#define _ESIC_H_


// GR
//#define _DEBUG 1
#ifndef _WIN32

	#define ESIC_TYPES
	#include <stdint.h>
	typedef uint8_t BYTE;
	typedef int16_t SWORD;
	typedef uint16_t  WORD;
	typedef int32_t SDWORD;
	typedef uint32_t DWORD;
	typedef uint8_t BOOL;
	typedef uint64_t QWORD;
	typedef int64_t SQWORD;
	typedef void VOID;
	typedef uint8_t UCHAR;
	typedef signed long int LONG;
	typedef unsigned long int ULONG;
	typedef unsigned short	WCHAR;
	
	/* These types must be 16-bit, 32-bit or larger integer */
	typedef int             INT;
	typedef unsigned int	UINT;

#else
	#include <Windows.h>
	#include <stdint.h>

	#define ESIC_TYPES
	typedef int16_t SWORD;
	typedef int32_t SDWORD;
#endif
	
#define ESIC_MACROS
#define HIWORD(value) ((value >> 16) & 0xFFFF)
#define LOWORD(value) (value & 0xFFFF)

	

#define RGB_16B( r, g, b ) \
   ( ( ( ( (DWORD)(r) >> 3u ) & 0x1fu ) << 11u ) | \
     ( ( ( (DWORD)(g) >> 2u ) & 0x3fu ) <<  5u ) | \
     ( ( ( (DWORD)(b) >> 3u ) & 0x1fu ) <<  0u ) )

#define RED(value)		((value >> 16) & 0xFF)
#define GREEN(value)	((value >> 8) & 0xFF)
#define BLUE(value) 	((value) & 0xFF)




#endif
