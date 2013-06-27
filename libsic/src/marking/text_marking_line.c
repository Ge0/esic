#include <libsic/marking/text_marking_line.h>

VTABLE_START(Object) {
	TextMarkingLine_destructor,
	TextMarkingLine_clone,
	TextMarkingLine_equalsTo,
	TextMarkingLine_hash,
	TextMarkingLine_type
};

PTextMarkingLine TextMarkingLine_constructor(PTextMarkingLine self) {
	return NULL;
}



void TextMarkingLine_destructor(PObject self) {
	/* TODO. */
}

PObject TextMarkingLine_clone(PObject self, PObject dst) {
	/* TODO. */
	return NULL;
}

BOOL TextMarkingLine_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD TextMarkingLine_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD TextMarkingLine_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("TextMarkingLine");
	}
	return s_hash;
}