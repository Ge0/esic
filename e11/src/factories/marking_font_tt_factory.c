#include <string.h>
#include <fatfs/ff.h>
#include <esic/etools/zstring.h>
#include <factories/marking_font_tt_factory.h>
#include <esic/eapi/heap.h>

/* Private functions */
PMarkingFontTT _build_font_tt(const char* name);
void _font_tt_hydrate_characters(PMarkingFontTT self, FIL* file);

static Hashtable s_fonts_tt;

void MarkingFontTTFactory_init() {
	Hashtable_constructor(&s_fonts_tt, 1, sizeof(ZString), sizeof(MarkingFontTT));
}

void MarkingFontTTFactory_destroy() {
	Hashtable_destructor(OBJECT(&s_fonts_tt));
}

PMarkingFontTT MarkingFontTTFactory_getMarkingFontTT(const char* font_name) {
	ZString key;
	PMarkingFontTT return_font = NULL;
	/* First: check whether the font has been loaded into memory or not */
	ZString_constructor(&key, font_name);

	if(!Hashtable_containsKey(&s_fonts_tt.map, &key.object)) {
		/* If not, load the font & add it to the map */
		return_font = _build_font_tt(font_name);

		if(return_font != NULL) {
			/* Put in the hashtable & point into it */
			Hashtable_put(&s_fonts_tt.map, &key.object, &return_font->object);
			DELETE(return_font);
			
		}
	}

	/* Finally: get the font into the hashtable */
	return_font = (PMarkingFontTT)Hashtable_get(&s_fonts_tt.map, &key.object);

	ZString_destructor(&key.object);

	return return_font;
}


	/* 
	char* path;
	DWORD path_length = strlen(PATH_MARKING_FONTS) + strlen(font_name) + strlen(MARKING_FONT_TT_EXT) + 1;
	FIL font_file;
	PMarkingFontTT font;

	path = (char*)malloc(path_length * sizeof(char));
	SicAssert(path != NULL);
	
	memset(path, '\0', path_length);
	strcpy(path, PATH_MARKING_FONTS);
	strcat(path, font_name);
	strcat(path, MARKING_FONT_TT_EXT);


	if(f_open(&font_file, path, FA_READ) != FR_OK) {
	
		SicPrintfDebug("MarkingFontFactory_getMarkingFont(): Cannot open %s! Abort...\r\n", path);
		SicFree(path);
		return NULL;
	}

	return NULL;
}

*/

/* Temporary function */
PMarkingFontTT _build_font_tt(const char* name) {
  
	char* tmp_file_name = NULL;
	PMarkingFontTT new_font = NULL;

	/* For fatfs */
	FIL font_file;
	DWORD br; /* Bytes read */

	char sign_file[10]; /* Should be "FONT_TT_", aka the font's magic word */
	DWORD filename_length = strlen(PATH_MARKING_FONTS) + strlen(name) + strlen(MARKING_FONT_TT_EXT) + 1;


	/* First of all: make sure the name's length does not exceed LCD_FONT_FILENAME_LENGTH */
	if(strlen(name) < MARKING_FONT_TT_FILENAME_SIZE) {
		tmp_file_name =	(char*)SicAlloc(filename_length * sizeof(char));

		SicAssert(tmp_file_name != NULL);
		memset(tmp_file_name, '\0', filename_length);

		strcpy(tmp_file_name, PATH_MARKING_FONTS);
		strcat(tmp_file_name, name);
		strcat(tmp_file_name, MARKING_FONT_TT_EXT);
		
		/* Open the file */
		/*FILE* fFont = NULL;*/

		/*if((fFont = fopen(name, "rb")) == NULL) {*/
		if(f_open(&font_file, (const TCHAR*)tmp_file_name, FA_READ) != FR_OK) {
			/* Error... */
			SicPrintfDebug("_build_font_tt(): Can't open %s!\n", name);
			SicFree(tmp_file_name);
			return NULL;
		}

		/* First step: is the file a font tt one? */
		f_read(&font_file, sign_file, 10, &br);

		if(strcmp(sign_file, MARKING_FONT_TT_SIGN)) {
			/* Error... */
			SicPrintfDebug("_build_font_tt(): %s is not any valid FONT_TT file. (Read %s instead)\n", name, sign_file);
			SicFree(tmp_file_name);
			return NULL;
		}

		NEW(new_font, MarkingFontTT);

		/* Ensure the allocation succeeded */
		SicAssert(new_font != NULL);

		/* Assign the file name */
		strncpy(new_font->font_name, name, MARKING_FONT_TT_FILENAME_SIZE);

		/* load characters into memory */
		_font_tt_hydrate_characters(new_font, &font_file);

		SicFree(tmp_file_name);
		f_close(&font_file);

	}

	return new_font;
}

void _font_tt_hydrate_characters(PMarkingFontTT self, FIL* file) {
	DWORD i;
	DWORD br;
	WORD offsets[MARKING_FONT_TT_CHARACTERS+1];
	f_read(file, offsets, (MARKING_FONT_TT_CHARACTERS+1) * sizeof(WORD), &br);

	/* Reverse offsets */
	for(i = 0; i < MARKING_FONT_TT_CHARACTERS+1; ++i) {
		offsets[i] = SWAP_WORD(offsets[i]);
	}


	/* Read the height of every character */
	f_lseek(file,offsets[0]-1);
	f_read(file, &self->height, sizeof(BYTE), &br);

	for(i = 1; i < MARKING_FONT_TT_CHARACTERS+1; ++i) {


		DWORD size = offsets[i] - offsets[i-1];
		if(size > 0) {
			DWORD number_of_points = (size - 2) / sizeof(MarkingFontTTCoordinate);

			/* Go to the offset */
			f_lseek(file, offsets[i-1]);

			self->characters[i-1].number_of_points = number_of_points;
			self->characters[i-1].coords = (PMarkingFontTTCoordinate)SicAlloc(number_of_points * sizeof(MarkingFontTTCoordinate));

			//SicPrintfDebug("%08X\n", number_of_points);
			SicAssert(self->characters[i-1].coords != NULL);

			/* Read both the offset and the width that defines every single character in the font */
			f_read(file, &self->characters[i-1].offset, sizeof(BYTE), &br); 
			f_read(file, &self->characters[i-1].width, sizeof(BYTE), &br);

			/* Read coords (x1,y1) & (x2,y2) of every points */
			f_read(file, self->characters[i-1].coords, sizeof(MarkingFontTTCoordinate) * number_of_points, &br);
		} else {
			self->characters[i-1].number_of_points = 0;
			self->characters[i-1].coords = NULL;
		}
	}
}