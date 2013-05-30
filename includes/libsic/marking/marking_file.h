#ifndef _MARKING_FILE_H_
#define _MARKING_FILE_H_

#include <esic/etools/vector.h>
#include <libsic/marking/marking_line.h>
#include <libsic/marking/marking_variable.h>

typedef struct _MarkingFile {
	Object object;
	Vector marking_lines;
	Vector marking_variables;
} MarkingFile, *PMarkingFile;

PMarkingFike MarkingFile_constructor(PMarkingFile self);

/* Object virtual functions */
void MarkingFile_destructor(PObject self);
PObject MarkingFile_clone(PObject self, PObject dst);
BOOL MarkingFile_equalsTo(PObject self, PObject dst);
DWORD MarkingFile_hash(PObject self);

#endif /* _MARKING_FILE_H_ */