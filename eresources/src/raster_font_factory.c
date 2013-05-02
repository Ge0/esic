/**
 * \file raster_font_factory.c
 */
#include <esic/etools/zstring.h>
#include <esic/etools/hashtable.h>
#include <esic/eresources/raster_font_factory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fatfs/ff.h>

/*
void _raster_font_read_header(PRasterFont self, FILE* fp);
void _raster_font_read_table_headers(PRasterFont self, FILE* fp);
void _raster_font_hydrate_tables(PRasterFont self, FILE* fp);
*/

PRasterFont _build_font(const char* name);
void _raster_font_read_header(PRasterFont self, FIL* font_file);
void _raster_font_read_table_headers(PRasterFont self, FIL* font_file);
void _raster_font_hydrate_tables(PRasterFont self, FIL* font_file);

static Hashtable s_fonts;

void RasterFontFactory_init() {
	Hashtable_constructor(&s_fonts, 3, sizeof(ZString), sizeof(RasterFont));
}

void RasterFontFactory_destroy() {
	Hashtable_destructor(&s_fonts.map.object);
}

PRasterFont RasterFontFactory_getRasterFont(const char* font_name) {
	ZString key;
	PRasterFont return_font = NULL;
	/* First: check whether the font has been loaded into memory or not */
	ZString_constructor(&key, font_name);

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
	return_font = (PRasterFont)Hashtable_get(&s_fonts.map, &key.object);

	ZString_destructor(&key.object);

	return return_font;
}

/* Temporary function */
PRasterFont _build_font(const char* name) {
  
	char* tmp_file_name = NULL;
	PRasterFont new_font = NULL;
	FIL fontFile;

	/* First of all: make sure the name's length does not exceed LCD_FONT_FILENAME_LENGTH */
	if(strlen(name) < RASTER_FONT_FILENAME_LENGTH) {
		tmp_file_name = (char*)SicAlloc((strlen(PATH_RASTER_FONTS) + strlen(name) + 1) * sizeof(char));

		//assert(tmp_file_name != NULL);

		strcpy(tmp_file_name, PATH_RASTER_FONTS);
		strcat(tmp_file_name, name);
		
		/* Open the file */
		/*FILE* fFont = NULL;*/

		/*if((fFont = fopen(name, "rb")) == NULL) {*/
		if(f_open(&fontFile, (const TCHAR*)tmp_file_name, FA_READ) != FR_OK) {
			/* Error... */
			fprintf(stderr, "Can't open %s!\n", name);
			SicFree(tmp_file_name);
			return NULL;
		}

		new_font = (PRasterFont)SicAlloc(sizeof(RasterFont));

		/* Ensure the allocation succeeded */
		//assert(new_font != NULL);

		/* Call the constructor */
		RasterFont_constructor(new_font);

		/* Assign the name */
		strcpy(new_font->name, name);

		/* Hydrate the data */

		/* First: read the header */
/*
		_raster_font_read_header(new_font, fFont);
*/
		_raster_font_read_header(new_font, &fontFile);

		/* Second: read table headers */
/*
		_raster_font_read_table_headers(new_font, fFont);
*/
		_raster_font_read_table_headers(new_font, &fontFile);
		
		/* Finally: get the tables */
/*
		_raster_font_hydrate_tables(new_font, fFont);
*/
		_raster_font_hydrate_tables(new_font, &fontFile);

		SicFree(tmp_file_name);

		f_close(&fontFile);

	}

	return new_font;
}

/*
void _raster_font_read_header(PRasterFont self, FILE* fp) {
*/
void _raster_font_read_header(PRasterFont self, FIL* font_file) {
	UINT br; /* Number of bytes read */
	f_read(font_file, &self->header, sizeof(RasterFontHeader), &br);
/*
	fread(&self->header, sizeof(RasterFontHeader), 1, fp);
*/
}

/*
void _raster_font_read_table_headers(PRasterFont self, FILE* fp) {
*/
void _raster_font_read_table_headers(PRasterFont self, FIL* font_file) {
	DWORD i = 0;
	UINT br; /* Number of bytes read */
	for(i; i < self->header.number_of_tables; ++i) {
		RasterFontTable table;

		/* Construct the object */
		RasterFontTable_constructor(&table);

/*
		fread(&table.header, sizeof(RasterFontTableHeader), 1, fp);
*/
		f_read(font_file, &table.header, sizeof(RasterFontTableHeader), &br);
		Vector_pushBack(&self->tables.container, &table.object);

		/* Destruct the object */
		RasterFontTable_destructor(&table.object);
	}
}

/*
void _raster_font_hydrate_tables(PRasterFont self, FILE* fp) {
*/
void _raster_font_hydrate_tables(PRasterFont self, FIL* font_file) {
	DWORD i = 0;
	UINT br;
	for(i; i < self->header.number_of_tables; ++i) {
		PRasterFontTable current_table = (PRasterFontTable)Vector_at(&self->tables.container, i);

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
		//assert(current_table->data != NULL);

/*		
		fseek(fp, current_table->header.offset, SEEK_SET);
		fread(current_table->data, sizeof(BYTE), size_to_alloc, fp);
*/
		f_lseek(font_file, current_table->header.offset);
		f_read(font_file, current_table->data, size_to_alloc, &br);
	}

}
