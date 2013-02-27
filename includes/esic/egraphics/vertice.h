/**
 * \file vertice.h
 */
#ifndef _VERTICE_H_
#define _VERTICE_H_

#include <esic/object.h>

typedef struct _Vertice {
	Object object;
	WORD x;
	WORD y;
} Vertice, *PVertice;

PVertice Vertice_constructor(PVertice self);

/* Virtual functions */
/* Object */
void Vertice_destructor(PObject self);
PObject Vertice_clone(PObject self, PObject dst);

#endif /* _VERTICE_H_ */