#ifndef _E11_TYPES_H_
#define _E11_TYPES_H_

// GR
//#define _DEBUG 1

/*-------------------------------------------------------------------------*/
/* Types de base                                                           */
/*-------------------------------------------------------------------------*/


#ifndef _WIN32

#include <stdint.h>
typedef unsigned char BYTE ;       /* entier 8 bits non sign� */
typedef int16_t SWORD ;   /* entier 16 bits sign� */
typedef uint16_t  WORD ;  /* entier 16 bits non sign� */
typedef int32_t SDWORD ;   /* entier 32 bits sign� */
typedef uint32_t DWORD ;  /* entier 32 bits non sign� */
typedef unsigned short BOOL ;  /* bool�en */
typedef unsigned long long QWORD;
typedef signed long long SQWORD;

   /* Note : pour compatibilit� avec Lib68331.h, LibE7.h et LibE9.h */

typedef signed char CHAR ;         /* entier 8 bits sign� */
typedef unsigned char UCHAR ;      /* entier 8 bits non sign� */

typedef signed short int INT ;     /* entier 16 bits sign� */
typedef unsigned short int UINT ;  /* entier 16 bits non sign� */

typedef signed long int LONG ;     /* entier 32 bits sign� */
typedef unsigned long int ULONG ;  /* entier 32 bits non sign� */

#else
#include <Windows.h>
#include <stdint.h>

typedef int16_t SWORD;
typedef int32_t SDWORD ;   /* entier 32 bits sign� */

#endif

/*-------------------------------------------------------------------------*/
/* Constantes et macros de base                                            */
/*-------------------------------------------------------------------------*/

                                   /* signature pour message description application */
#define PGF_SIGNATDESCU "PG_FLASH647B97BAE51A5AAA0C8519U\0"

#ifndef FALSE
#define FALSE  0                   /* bool�en faux (BOOL) */
#endif /* FALSE */

#ifndef TRUE
#define TRUE   1                   /* bool�en vrai (BOOL) */
#endif /* TRUE */

#ifndef NULL
#define NULL   0                   /* pointeur nul */
#endif /* NULL */

#endif
