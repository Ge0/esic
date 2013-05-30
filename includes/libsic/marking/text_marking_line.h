#ifndef _TEXT_MARKING_LINE_H_
#define _TEXT_MARKING_LINE_H_

#include <libsic/marking/marking_line.h>

typedef struct _TextMarkingLine {
	MarkingLine marking_line;
	int pitch; /* pitch between 2 chars */
	/* TODO: embed pointer to the sic font */
} TextMarkingLine, *PTextMarkingLine;

PTextMarkingLine TextMarkingLine_constructor(PTextMarkingLine self);

/* Object virtual functions */
void TextMarkingLine_destructor(PObject self);
PObject TextMarkingLine_clone(PObject self, PObject dst);
BOOL TextMarkingLine_equalsTo(PObject self, PObject dst);
DWORD TextMarkingLine_hash(PObject self);

#endif /* _TEXT_MARKING_LINE_H_ */