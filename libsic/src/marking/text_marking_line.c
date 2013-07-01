#include <libsic/marking/text_marking_line.h>

VTABLE_START(Object) {
	TextMarkingLine_destructor,
	TextMarkingLine_clone,
	TextMarkingLine_equalsTo,
	TextMarkingLine_hash,
	TextMarkingLine_type
};

PTextMarkingLine TextMarkingLine_constructor(PTextMarkingLine self) {
	OBJECT(self)->size   = sizeof(TextMarkingLine);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	ZString_constructor(&self->content, "");

	return self;
}



void TextMarkingLine_destructor(PObject self) {
	/* TODO. */
	ZString_destructor(OBJECT(&TEXTMARKINGLINE(self)->content));
}

PObject TextMarkingLine_clone(PObject self, PObject dst) {
	/* TODO. */
	dst->size   = self->size;
	dst->vtable = self->vtable;

	MARKINGLINE(dst)->x        = MARKINGLINE(self)->x;
	MARKINGLINE(dst)->y        = MARKINGLINE(self)->y;
	MARKINGLINE(dst)->z        = MARKINGLINE(self)->z;
	MARKINGLINE(dst)->d        = MARKINGLINE(self)->d;
	MARKINGLINE(dst)->speed    = MARKINGLINE(self)->speed;
	MARKINGLINE(dst)->strength = MARKINGLINE(self)->strength;
	MARKINGLINE(dst)->width    = MARKINGLINE(self)->width;
	MARKINGLINE(dst)->height   = MARKINGLINE(dst)->height;

	ZString_clone(OBJECT(&TEXTMARKINGLINE(self)->content), OBJECT(&TEXTMARKINGLINE(dst)->content));

	TEXTMARKINGLINE(dst)->marking_font = TEXTMARKINGLINE(self)->marking_font;
	TEXTMARKINGLINE(dst)->pitch        = TEXTMARKINGLINE(self)->pitch;

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