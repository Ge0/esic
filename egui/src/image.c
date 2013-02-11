#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/image.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_object_vtable = {
	Image_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	Image_paint
};

PImage Image_constructor(PImage self) {

	/* Filling vtables */

	/* Constructing members */
	SzString_constructor(&self->text, "");

	return self;
}

void Image_destructor(PObject self) {
	PImage real_self = (PImage)self;

	/* Calling parent destructor */
	//Widget_destructor(self);

	/* Destructing members */
	SzString_destructor(&real_self->text.object);
}

void Image_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	DefaultWidgetRenderer_paintImage((PImage)self, base_x, base_y);
}

void createRawImageFrom2DBuffer(PImage self, const char* buffer, WORD pHeight, WORD pWidth, BPP bpp)
{
	WORD i,j;
	/* create buffer on the heap */
	/* Ensure the two mallocs succeeded */
	self->bpp = bpp;
	switch(bpp){
	case BPP1:
		self->rawBuffer = (char*)SicAlloc(pHeight*pWidth);
		memset(self->rawBuffer, 0, pHeight*pWidth);
		break;
	case BPP8:
		self->rawBuffer = (char*)SicAlloc(pHeight*pWidth);
		memset(&self->rawBuffer, 0, pHeight*pWidth);
		break;
	case BPP16:
		self->rawBuffer = (char*)SicAlloc(pHeight*pWidth*2);
		memset(&self->rawBuffer, 0, pHeight*pWidth*2);
		break;
	case BPP24:
		self->rawBuffer = (char*)SicAlloc(pHeight*pWidth*3);
		memset(&self->rawBuffer, 0, pHeight*pWidth*3);
		break;
	}
	  
	assert(self->rawBuffer != NULL);	

	self->width = pWidth;
	self->height = pHeight;

	/* flat buffer copy */
	for ( i = 0; i < pHeight; i++)	//row
	{
		for ( j = 0; j < pWidth; j++)	//column
		{
			self->rawBuffer[i * pWidth + j] = buffer[i * pWidth + j];			
		}
	}
}