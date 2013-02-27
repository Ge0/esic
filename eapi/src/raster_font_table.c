/**
 * \file raster_font_table.c
 */
#include <esic/eapi/raster_font_table.h>
#include <string.h>
#include <math.h>

/* Vtables definition */
static const vtable_Object s_object_vtable = {
	RasterFontTable_destructor,
	RasterFontTable_clone,
	NULL,
	NULL
};


PRasterFontTable RasterFontTable_constructor(PRasterFontTable self) {
	/* Call the parent constructor */
	/* Base structure is Object: no need to construct (Object is abstract!) */

	/* Filling the size member */
	self->object.size = sizeof(RasterFontTable);

	/* Filling object's vtable */
	/*
	self->object.vtable.destructor	= RasterFontTable_destructor;
	self->object.vtable.clone		= RasterFontTable_clone;
	*/
	self->object.vtable = &s_object_vtable;

	self->data = NULL;
	
	memset(&self->header, 0, sizeof(RasterFontTableHeader));

	return self;

}

PObject RasterFontTable_clone(PObject self, PObject dst) {
	PRasterFontTable real_self = (PRasterFontTable)self;
	PRasterFontTable real_dst  = (PRasterFontTable)dst;
	DWORD size_to_alloc = 0;

	real_dst->object = real_self->object;

	/* Copying members */
	real_dst->header = real_self->header;

	if(real_self->data != NULL) {

		/* Calculating size to alloc for the data */
		size_to_alloc = real_self->header.character_width * real_self->header.character_height;

		/* Force 8-bit alignement */
		if(size_to_alloc % 8) {
			size_to_alloc += (8 - (size_to_alloc % 8));
		}

		size_to_alloc *= real_self->header.number_of_characters;

		/* Adjusting size_to_alloc */
		size_to_alloc = (DWORD)ceil(size_to_alloc / 8.0);

		/* 32 bits final alignment forcing */
		if(size_to_alloc % 4) {
			size_to_alloc += (4 - (size_to_alloc % 4));
		}


		real_dst->data = SicAlloc(size_to_alloc * sizeof(BYTE));
		memcpy(real_dst->data, real_self->data, size_to_alloc);
	} else {
		real_dst->data = NULL;
	}

	return (PObject)real_dst;
}

void RasterFontTable_destructor(PObject self) {
	PRasterFontTable real_self = (PRasterFontTable)self;
	if(real_self->data != NULL) {
		SicFree(real_self->data);
		real_self->data = NULL;
	}
}