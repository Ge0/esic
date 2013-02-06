#include <esic/etools/szstring.h>
#include <esic/etools/hashtable.h>
#include <esic/elcd/lcd_font_factory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fatfs/ff.h>

/*
void _lcd_font_read_header(PLcdFont self, FILE* fp);
void _lcd_font_read_table_headers(PLcdFont self, FILE* fp);
void _lcd_font_hydrate_tables(PLcdFont self, FILE* fp);
*/

PLcdFont _build_font(const char* name);
void _lcd_font_read_header(PLcdFont self, FIL* font_file);
void _lcd_font_read_table_headers(PLcdFont self, FIL* font_file);
void _lcd_font_hydrate_tables(PLcdFont self, FIL* font_file);

static Hashtable s_fonts;

void LcdFontFactory_init() {
	Hashtable_constructor(&s_fonts, 3, sizeof(SzString), sizeof(LcdFont));
}

void LcdFontFactory_destroy() {
	Hashtable_destructor(&s_fonts.map.object);
}

PLcdFont LcdFontFactory_getLcdFont(const char* font_name) {
	SzString key;
	PLcdFont return_font = NULL;
	/* First: check whether the font has been loaded into memory or not */
	SzString_constructor(&key, font_name);

	if(!Hashtable_containsKey(&s_fonts.map, &key.object)) {
		/* If not, load the font & add it to the map */
		return_font = _build_font(font_name);

		if(return_font != NULL) {
			/* Put in the hashtable & point into it */
			Hashtable_put(&s_fonts.map, &key.object, &return_font->object);
			DELETE(return_font);
			
		}
	}

	/* Finally: get the font into the hashtable */
	return_font = (PLcdFont)Hashtable_get(&s_fonts.map, &key.object);

	SzString_destructor(&key.object);

	return return_font;
}

/* Temporary function */
PLcdFont _build_font(const char* name) {
  
	char* tmp_file_name = NULL;
	PLcdFont new_font = NULL;
	FIL fontFile;

	/* First of all: make sure the name's length does not exceed LCD_FONT_FILENAME_LENGTH */
	if(strlen(name) < LCD_FONT_FILENAME_LENGTH) {
		tmp_file_name = (char*)SicAlloc((strlen(PATH_LCD_FONTS) + strlen(name) + 1) * sizeof(char));

		assert(tmp_file_name != NULL);

		strcpy(tmp_file_name, PATH_LCD_FONTS);
		strcat(tmp_file_name, name);
		
		/* Open the file */
		/*FILE* fFont = NULL;*/

		/*if((fFont = fopen(name, "rb")) == NULL) {*/
		if(f_open(&fontFile, (const TCHAR*)tmp_file_name, FA_READ) != FR_OK) {
			/* Error... */
			fprintf(stderr, "Can't open %s!\n", name);
			return NULL;
		}

		new_font = (PLcdFont)SicAlloc(sizeof(LcdFont));

		/* Ensure the allocation succeeded */
		assert(new_font != NULL);

		/* Call the constructor */
		LcdFont_constructor(new_font);

		/* Assign the name */
		strcpy(new_font->name, name);

		/* Hydrate the data */

		/* First: read the header */
/*
		_lcd_font_read_header(new_font, fFont);
*/
		_lcd_font_read_header(new_font, &fontFile);

		/* Second: read table headers */
/*
		_lcd_font_read_table_headers(new_font, fFont);
*/
		_lcd_font_read_table_headers(new_font, &fontFile);
		
		/* Finally: get the tables */
/*
		_lcd_font_hydrate_tables(new_font, fFont);
*/
		_lcd_font_hydrate_tables(new_font, &fontFile);

		SicFree(tmp_file_name);

		f_close(&fontFile);

	}

	return new_font;
}

/*
void _lcd_font_read_header(PLcdFont self, FILE* fp) {
*/
void _lcd_font_read_header(PLcdFont self, FIL* font_file) {
	UINT br; /* Number of bytes read */
	f_read(font_file, &self->header, sizeof(LcdFontHeader), &br);
/*
	fread(&self->header, sizeof(LcdFontHeader), 1, fp);
*/
}

/*
void _lcd_font_read_table_headers(PLcdFont self, FILE* fp) {
*/
void _lcd_font_read_table_headers(PLcdFont self, FIL* font_file) {
	DWORD i = 0;
	UINT br; /* Number of bytes read */
	for(i; i < self->header.number_of_tables; ++i) {
		LcdFontTable table;

		/* Construct the object */
		LcdFontTable_constructor(&table);

/*
		fread(&table.header, sizeof(LcdFontTableHeader), 1, fp);
*/
		f_read(font_file, &table.header, sizeof(LcdFontTableHeader), &br);
		Vector_pushBack(&self->tables.container, &table.object);

		/* Destruct the object */
		LcdFontTable_destructor(&table.object);
	}
}

/*
void _lcd_font_hydrate_tables(PLcdFont self, FILE* fp) {
*/
void _lcd_font_hydrate_tables(PLcdFont self, FIL* font_file) {
	DWORD i = 0;
	UINT br;
	for(i; i < self->header.number_of_tables; ++i) {
		PLcdFontTable current_table = (PLcdFontTable)Vector_at(&self->tables.container, i);

		/* Calculate the size to allocate for the data */
		DWORD size_to_alloc = current_table->header.character_width * current_table->header.character_height;

		/* Force 8-bit alignement */
		if(size_to_alloc % 8) {
			size_to_alloc += (8 - (size_to_alloc % 8));
		}

		size_to_alloc *= current_table->header.number_of_characters;

		/* Adjusting size_to_alloc */
		size_to_alloc = (DWORD)ceil(size_to_alloc / 8.0);

		/* 32 bits final alignment forcing */
		if(size_to_alloc % 4) {
			size_to_alloc += (4 - (size_to_alloc % 4));
		}

		current_table->data = SicAlloc(size_to_alloc * sizeof(BYTE));

		/* Ensure the allocation succeeded */
		assert(current_table->data != NULL);

/*		
		fseek(fp, current_table->header.offset, SEEK_SET);
		fread(current_table->data, sizeof(BYTE), size_to_alloc, fp);
*/
		f_lseek(font_file, current_table->header.offset);
		f_read(font_file, current_table->data, size_to_alloc, &br);
	}

}
