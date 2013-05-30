#ifndef _INCREMENT_MARKING_VARIABLE_H_
#define _INCREMENT_MARKING_VARIABLE_H_

#include <libsic/marking/marking_variable.h>

typedef struct _IncrementMarkingVariable {
	MarkingVariable marking_variable;
	DWORD step;
	DWORD frequency;
	DWORD value;
} IncrementMarkingVariable;

PIncrementMarkingVariable IncrementMarkingVariable_constructor(PIncrementMarkingLine self);

/* Object virtual functions */
void IncrementMarkingVariable_destructor(PObject self);
PObject IncrementMarkingVariable_clone(PObject self, PObject dst);
BOOL IncrementMarkingVariable_equalsTo(PObject self, PObject dst);
DWORD IncrementMarkingVariable_hash(PObject self);

#endif /* _INCREMENT_MARKING_VARIABLE_H_ */