/**
 * \file vertice.h
 */
#ifndef _VERTICE_H_
#define _VERTICE_H_

#define VERTICE_SWAP(v1, v2) \
	SWAP(v1->x, v2->x); \
	SWAP(v1->y, v2->y);

#include <esic/object.h>

typedef struct _Vertice {
	Object object;
	DWORD x;
	DWORD y;
} Vertice, *PVertice;

PVertice Vertice_constructor(PVertice self);

/* Virtual functions */
/* Object */
void Vertice_destructor(PObject self);
PObject Vertice_clone(PObject self, PObject dst);

#endif /* _VERTICE_H_ */