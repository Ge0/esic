#include <esic/elcd/lcd_font.h>
#include <esic/elcd/lcd_font_table_header.h>
#include <esic/etools/vector.h>
#include <math.h>
#include <string.h>

static const vtable_Object s_lcdfont_object_vtable = {
  LcdFont_destructor,
  LcdFont_clone,
  NULL,
  NULL
};

PLcdFont LcdFont_constructor(PLcdFont self) {
	/* Call the parent constructor */
	
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* Filling the size member */
	self->object.size = sizeof(LcdFont);

	/* Filling object's vtable */
	/*
	self->object.vtable.destructor	= LcdFont_destructor;
	self->object.vtable.clone		= LcdFont_clone;
	*/
	self->object.vtable = &s_lcdfont_object_vtable;

	/* Construct the vector */
	Vector_constructor(&self->tables, sizeof(LcdFontTable));

	return self;
}

PObject LcdFont_clone(PObject self, PObject dst) {
	
	PLcdFont real_self = (PLcdFont)self;
	PLcdFont real_dst  = (PLcdFont)dst;


	/* Copying members */
	real_dst->object = real_self->object;
	memset(real_dst->name, '\0', LCD_FONT_FILENAME_LENGTH);
	strncpy(real_dst->name, real_self->name, LCD_FONT_FILENAME_LENGTH-1);
	real_dst->header = real_self->header;

	/* TODO. */
	dst = real_self->tables.container.object.vtable->clone(
		&real_self->tables.container.object,
		&real_dst->tables.container.object
	);


	return self;
}

void LcdFont_destructor(PObject self) {
	PLcdFont real_self = (PLcdFont)self;

	/* Destruct the vector that contains the table of characters */
	Vector_destructor(&real_self->tables.container.object);

}

void _lcd_font_init(PLcdFont self) {
	Vector_constructor(&self->tables, sizeof(LcdFontTable));
}

char* LcdFont_getCharacterData(PLcdFont self, DWORD utf8_code, DWORD* size, BYTE* unit_width, BYTE* unit_height) {
	/* First of all, locate the appropriated table */
	DWORD i;
	char* character_data		= NULL;
	PLcdFontTable current_table	= NULL;

	/* Retrieve the appropriated table */
	for(i = 0; i < self->header.number_of_tables, current_table == NULL; ++i) {
		current_table = (PLcdFontTable)Vector_at(&self->tables.container, i);

		/* If the current utf8 code is within the table, it means that we found it */
		if(utf8_code >= current_table->header.utf8_base_code_point && utf8_code <=
			current_table->header.utf8_base_code_point + current_table->header.number_of_characters) {
				PLcdFontTable current_table = (PLcdFontTable)Vector_at(&self->tables.container, i);
		}
	}

	/* table found? Locate the data, copy it into our buffer and return it */
	if(current_table != NULL) {
		/* How many bytes per character ? */
		DWORD bytes_per_character = current_table->header.character_height * current_table->header.character_width;
		DWORD character_index = -1;
		
		/* 8-bit alignment */
		if(bytes_per_character % 8) {
			bytes_per_character += (8 - (bytes_per_character % 8));
		}

		bytes_per_character /= 8;

		/* Allocation */
		character_data = (char*) SicAlloc( bytes_per_character * sizeof(char));

		/* Ensure the allocation succeeded */
		assert(character_data != NULL);

		/* Calculate the character's index */
		character_index = utf8_code - current_table->header.utf8_base_code_point;

		/* Copy the memory content */
		memcpy(character_data, (const char*)current_table->data + (character_index * bytes_per_character), bytes_per_character);

		*size			= bytes_per_character;
		*unit_width		= current_table->header.character_width;
		*unit_height	= current_table->header.character_height;
	}

	return character_data;
}