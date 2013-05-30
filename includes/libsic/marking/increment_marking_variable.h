#ifndef _INCREMENT_MARKING_VARIABLE_H_
#define _INCREMENT_MARKING_VARIABLE_H_

#include <libsic/marking/increment_marking_variable.h>

typedef struct _IncrementMarkingVariable {
	MarkingVariable marking_variable;
	DWORD step;
	DWORD frequency;
	DWORD value;
}

#endif /* _INCREMENT_MARKING_VARIABLE_H_ */