#ifndef _MISC_H_
#define _MISC_H_

#include <esic/esic.h>

#define SWAP_WORD(x) ((x >> 8) | ((x & 0xFF) << 8))

int Sign(SDWORD expr);
unsigned long Hash(const char* string);
long RandLong(long min, long max);
long CrossProduct(DWORD x1, DWORD y1, DWORD x2, DWORD y2);

#endif /* _MISC_H_ */