#include <esic/egraphics/pixel.h>

VTABLE_START(Object) {
	Pixel_destructor,
	Pixel_clone,
	Pixel_equalsTo,
	Pixel_hash,
	Pixel_type
};

VTABLE_START(Shape) {
	Pixel_paint,
	Pixel_translateTo
};

PPixel Pixel_constructor(PPixel self) {

	/* Calling parent constructor */
	Shape_constructor(SHAPE(self));

	OBJECT(self)->size = sizeof(Pixel);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	SHAPE(self)->vtable = VTABLE_POINTER(Shape);

	Vertice_constructor(&self->shape.position);

	return self;
}

/* Virtual functions */
/* Object */
void Pixel_destructor(PObject self) {
	/* Nothing to do. */
}

PObject Pixel_clone(PObject self, PObject dst) {
	/* TODO. */

	return NULL;
}


BOOL Pixel_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Pixel_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD Pixel_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Pixel");
	}

	return s_hash;
}

/* Shape */
void Pixel_paint(PShape self, PPainter painter) {
	// Check wether the pixel is within the bounds of the painter's clip

	Painter_drawPixel(painter, self->position.x, self->position.y, PIXEL(self)->color);

}

void Pixel_translateTo(PShape self, SDWORD x, SDWORD y) {
	self->position.x += x;
	self->position.y += y;
}