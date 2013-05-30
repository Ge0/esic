#ifndef _MARKING_VARIABLE_H_
#define _MARKING_VARIABLE_H_

#include <esic/etools/zstring.h>

typedef struct _MarkingVariable {
	ZString name;
} MarkingVariable;

PMarkingFike MarkingVariable_constructor(PMarkingVariable self);

/* Object virtual functions */
void MarkingVariable_destructor(PObject self);
PObject MarkingVariable_clone(PObject self, PObject dst);
BOOL MarkingVariable_equalsTo(PObject self, PObject dst);
DWORD MarkingVariable_hash(PObject self);

#endif /* _MARKING_VARIABLE_H_ */