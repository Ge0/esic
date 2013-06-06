#ifndef _MISC_H_
#define _MISC_H_

#include <esic/esic.h>

#define SWAP_WORD(x) ((x >> 8) | ((x & 0xFF) << 8))

int Sign(SDWORD expr);
unsigned long Hash(const char* string);

#endif /* _MISC_H_ */