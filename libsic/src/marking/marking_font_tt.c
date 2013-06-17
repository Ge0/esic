#include <string.h>
#include <libsic/marking/marking_font_tt.h>

VTABLE_START(Object) {
	MarkingFontTT_destructor,
	MarkingFontTT_clone,
	MarkingFontTT_equalsTo,
	MarkingFontTT_hash,
	MarkingFontTT_type
};

PMarkingFontTT MarkingFontTT_constructor(PMarkingFontTT self) {

	OBJECT(self)->size   = sizeof(MarkingFontTT);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	memset(self->font_name, '\0', MARKING_FONT_TT_FILENAME_SIZE);

	return self;
}

/* Virtual functions */
/* Object */
void MarkingFontTT_destructor(PObject self) {
	DWORD i;

	for(i = 0; i < MARKING_FONT_TT_CHARACTERS; ++i) {
		SicFree( (MARKINGFONTTT(self)->characters[i].coords) );
	}
}
PObject MarkingFontTT_clone(PObject self, PObject dst) {
	DWORD i;

	/* First step: copy object properties */
	dst->size = self->size;
	dst->vtable = self->vtable;

	/* Second step: copy the font's details */
	memset(MARKINGFONTTT(dst)->font_name, '\0', MARKING_FONT_TT_FILENAME_SIZE);
	strncpy(MARKINGFONTTT(dst)->font_name, MARKINGFONTTT(self)->font_name, MARKING_FONT_TT_FILENAME_SIZE-1);
	MARKINGFONTTT(dst)->font_name[MARKING_FONT_TT_FILENAME_SIZE-1] = '\0';

	MARKINGFONTTT(dst)->height = MARKINGFONTTT(self)->height;

	/* Copy characters-wise information */
	for(i = 0; i < MARKING_FONT_TT_CHARACTERS; ++i) {
		/* Make the copy if and only if needed to do so */
		if(MARKINGFONTTT(self)->characters[i].coords != NULL) {
			MARKINGFONTTT(dst)->characters[i].offset           = MARKINGFONTTT(self)->characters[i].offset;
			MARKINGFONTTT(dst)->characters[i].number_of_points = MARKINGFONTTT(self)->characters[i].number_of_points;
			MARKINGFONTTT(dst)->characters[i].width            = MARKINGFONTTT(self)->characters[i].width;
			MARKINGFONTTT(dst)->characters[i].coords = (PMarkingFontTTCoordinate)SicAlloc(
				MARKINGFONTTT(dst)->characters[i].number_of_points * sizeof(MarkingFontTTCoordinate)
			);
		
			SicAssert(MARKINGFONTTT(dst)->characters[i].coords != NULL);

			memcpy(
				MARKINGFONTTT(dst)->characters[i].coords,
				MARKINGFONTTT(self)->characters[i].coords, 
				MARKINGFONTTT(dst)->characters[i].number_of_points * sizeof(MarkingFontTTCoordinate)
			);
		} else {
			MARKINGFONTTT(dst)->characters[i].coords = NULL;
			MARKINGFONTTT(dst)->characters[i].number_of_points = 0;
		}
	}

	return dst;

}

BOOL MarkingFontTT_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD MarkingFontTT_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD MarkingFontTT_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("MarkingFontTT");
	}

	return s_hash;
}
