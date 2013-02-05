#ifndef _E11_TYPES_H_
#define _E11_TYPES_H_

// GR
//#define _DEBUG 1

/*-------------------------------------------------------------------------*/
/* Types de base                                                           */
/*-------------------------------------------------------------------------*/


#ifndef _WIN32

#include <stdint.h>
typedef unsigned char BYTE ;       /* entier 8 bits non signé */
typedef int16_t SWORD ;   /* entier 16 bits signé */
typedef uint16_t  WORD ;  /* entier 16 bits non signé */
typedef int32_t SDWORD ;   /* entier 32 bits signé */
typedef uint32_t DWORD ;  /* entier 32 bits non signé */
typedef unsigned short BOOL ;  /* booléen */
typedef unsigned long long QWORD;
typedef signed long long SQWORD;

   /* Note : pour compatibilité avec Lib68331.h, LibE7.h et LibE9.h */

typedef signed char CHAR ;         /* entier 8 bits signé */
typedef unsigned char UCHAR ;      /* entier 8 bits non signé */

typedef signed short int INT ;     /* entier 16 bits signé */
typedef unsigned short int UINT ;  /* entier 16 bits non signé */

typedef signed long int LONG ;     /* entier 32 bits signé */
typedef unsigned long int ULONG ;  /* entier 32 bits non signé */

#else
#include <Windows.h>
#include <stdint.h>

typedef int16_t SWORD;
typedef int32_t SDWORD ;   /* entier 32 bits signé */

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
