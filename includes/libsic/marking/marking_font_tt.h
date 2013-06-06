#ifndef _MARKING_FONT_TT_H_
#define _MARKING_FONT_TT_H_

#include <esic/object.h>

#define MARKING_FONT_TT_FILENAME_SIZE	12
#define MARKING_FONT_TT_CHARACTERS		224

#define MARKING_FONT_TT_SIGN			"FONT_TT__"
#define MARKING_FONT_TT_SIGN1			0x544E4F46	/* "FONT" */
#define MARKING_FONT_TT_SIGN2			0x5F54545F	/* "_TT_" */

typedef struct _MarkingFontTTCoordinate* PMarkingFontTTCoordinate;
typedef struct _MarkingFontTTCoordinate {
	SBYTE x1;
	BYTE y1;
	SBYTE x2;
	BYTE y2;
} MarkingFontTTCoordinate;

typedef struct _MarkingFontTTCharacter *PMarkingFontTTCharacter;
typedef struct _MarkingFontTTCharacter {
	DWORD number_of_points;
	PMarkingFontTTCoordinate coords;
	BYTE offset;
	BYTE width;	/* Logical units */
} MarkingFontTTCharacter;

typedef struct _MarkingFontTT* PMarkingFontTT;

typedef struct _MarkingFontTT {
	Object object;
	char font_name[MARKING_FONT_TT_FILENAME_SIZE];
	MarkingFontTTCharacter characters[MARKING_FONT_TT_CHARACTERS];
	BYTE height;	/* Character's height */
} MarkingFontTT;

PMarkingFontTT MarkingFontTT_constructor(PMarkingFontTT self);

/* Virtual functions */
/* Object */
void MarkingFontTT_destructor(PObject self);
PObject MarkingFontTT_clone(PObject self, PObject dst);
BOOL MarkingFontTT_equalsTo(PObject self, PObject dst);
DWORD MarkingFontTT_hash(PObject self);
DWORD MarkingFontTT_type(PObject self);

#define MARKINGFONTTT(x) ((PMarkingFontTT)x)

#endif /* _MARKING_FONT_H_ */