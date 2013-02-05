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

void _lcd_font_read_header(PLcdFont self, FIL* font_file);
void _lcd_font_read_table_headers(PLcdFont self, FIL* font_file);
void _lcd_font_hydrate_tables(PLcdFont self, FIL* font_file);


/* Temporary function */
PLcdFont build_font(const char* name) {

	static const char* path_lcd_fonts = "system/lcd_fonts/";
  
	char* tmp_file_name = NULL;
	PLcdFont new_font = NULL;
	FIL fontFile;

	/* First of all: make sure the name's length does not exceed LCD_FONT_FILENAME_LENGTH */
	if(strlen(name) < LCD_FONT_FILENAME_LENGTH) {
		tmp_file_name = (char*)SicAlloc((strlen(path_lcd_fonts) + strlen(name) + 1) * sizeof(char));

		assert(tmp_file_name != NULL);

		strcpy(tmp_file_name, path_lcd_fonts);
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
