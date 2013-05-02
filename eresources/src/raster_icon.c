/**
 * \file raster_icon.c
 */
//#include <assert.h>
#include <esic/eapi/raster_icon.h>
#include <string.h>

static const vtable_Object s_object_vtable = {
	RasterIcon_destructor,
	RasterIcon_clone,
	NULL,
	NULL
};

PRasterIcon RasterIcon_constructor(PRasterIcon self) {

	self->object.size   = sizeof(RasterIcon);
	self->object.vtable = &s_object_vtable;

	return self;
}

void RasterIcon_destructor(PObject self) {
	PRasterIcon real_self = (PRasterIcon)self;

	if(real_self->data != NULL) {
		SicFree(real_self->data);
	}
}

PObject RasterIcon_clone(PObject self, PObject dst) {
	PRasterIcon real_self = (PRasterIcon)self;
	PRasterIcon real_dst  = (PRasterIcon)dst;
	DWORD size = 0;
	dst->size   = self->size;
	dst->vtable = self->vtable;

	real_dst->header = real_self->header;

	size = real_self->header.width * real_self->header.height * sizeof(WORD);

	real_dst->data = (BYTE*)SicAlloc( size );

	/* Ensure allocation succeeded */
	//assert(real_dst->data != NULL);

	memcpy(real_dst->data, real_self->data, size);

	return dst;
	
}