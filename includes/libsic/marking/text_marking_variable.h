#ifndef _TEXT_MARKING_VARIABLE_H_
#define _TEXT_MARKING_VARIABLE_H_

typedef struct _TextMarkingVariable {
	ZString text;
} TextMarkingVariable, *PTextMarkingVariable;

PTextMarkingVariable TextMarkingVariable_constructor(PTextMarkingLine self);

/* Object virtual functions */
void TextMarkingVariable_destructor(PObject self);
PObject TextMarkingVariable_clone(PObject self, PObject dst);
BOOL TextMarkingVariable_equalsTo(PObject self, PObject dst);
DWORD TextMarkingVariable_hash(PObject self);


#endif /* _TEXT_MARKING_VARIABLE_H_ */