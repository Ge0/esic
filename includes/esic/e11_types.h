#ifndef _E11_TYPES_H_
#define _E11_TYPES_H_

// GR
//#define _DEBUG 1
#ifndef _WIN32

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
typedef int8_t CHAR;
typedef uint8_t UCHAR;

typedef signed short int INT;
typedef unsigned short int UINT;

typedef signed long int LONG;
typedef unsigned long int ULONG;

#define HIWORD(value) ((value >> 16) & 0xFFFF)
#define LOWORD(value) (value & 0xFFFF)

#else
#include <Windows.h>
#include <stdint.h>

typedef int16_t SWORD;
typedef int32_t SDWORD;

// GR

#define RGB_16B( r, g, b ) \
   ( ( ( ( (DWORD)(r) >> 3u ) & 0x1fu ) << 11u ) | \
     ( ( ( (DWORD)(g) >> 2u ) & 0x3fu ) <<  5u ) | \
     ( ( ( (DWORD)(b) >> 3u ) & 0x1fu ) <<  0u ) )

#define RED(value)		((value >> 16) & 0xFF)
#define GREEN(value)	((value >> 8) & 0xFF)
#define BLUE(value) 	((value) & 0xFF)

#endif



/*-------------------------------------------------------------------------*/
/* Constantes et macros de base                                            */
/*-------------------------------------------------------------------------*/

                                   /* signature pour message description application */
#define PGF_SIGNATDESCU "PG_FLASH647B97BAE51A5AAA0C8519U\0"

#ifndef FALSE
#define FALSE  0                   /* booléen faux (BOOL) */
#endif /* FALSE */

#ifndef TRUE
#define TRUE   1                   /* booléen vrai (BOOL) */
#endif /* TRUE */

#ifndef NULL
#define NULL   0                   /* pointeur nul */
#endif /* NULL */

#endif
