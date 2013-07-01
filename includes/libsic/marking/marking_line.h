#ifndef _MARKING_LINE_H_
#define _MARKING_LINE_H_

#include <esic/esic.h>
#include <esic/object.h>

typedef struct _MarkingLine {
	Object  object;
	long   x;
	long   y;
	long   z;
	long   d;
	long   width;
	long   height;
	DWORD   speed;
	DWORD   strength;
} MarkingLine, *PMarkingLine;

PMarkingLine MarkingLine_constructor(PMarkingLine self);

/* Object virtual functions */
void MarkingLine_destructor(PObject self);
PObject MarkingLine_clone(PObject self, PObject dst);
BOOL MarkingLine_equalsTo(PObject self, PObject dst);
DWORD MarkingLine_hash(PObject self);

#define MARKINGLINE(x) ((PMarkingLine)x)

#endif /* _MARKING_LINE_H_ */