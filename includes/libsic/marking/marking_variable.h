#ifndef _MARKING_VARIABLE_H_
#define _MARKING_VARIABLE_H_

#include <esic/object.h>
#include <esic/etools/zstring.h>

typedef struct _MarkingVariable* PMarkingVariable;

typedef struct _MarkingVariable {
	Object object;
	ZString name;
} MarkingVariable;

PMarkingVariable MarkingVariable_constructor(PMarkingVariable self);

/* Object virtual functions */
void MarkingVariable_destructor(PObject self);
PObject MarkingVariable_clone(PObject self, PObject dst);
BOOL MarkingVariable_equalsTo(PObject self, PObject dst);
DWORD MarkingVariable_hash(PObject self);

#endif /* _MARKING_VARIABLE_H_ */