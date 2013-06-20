#include <esic/egraphics/painter.h>

VTABLE_START(Object) {
	Painter_destructor,
	Painter_clone,
	Painter_equalsTo,
	Painter_hash,
	Painter_type
};

PPainter Painter_constructor(PPainter self) {
	OBJECT(self)->size   = sizeof(Painter);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	/* By default, the clipping region is infinite */
	self->clip.x      = 0;
	self->clip.y      = 0;
	self->clip.width  = 0xFFFFFFFF;
	self->clip.height = 0xFFFFFFFF;

	/* And the color is black */
	self->color = RGB_16B(0,0,0);

	return self;
}

void Painter_destructor(PObject self) {
	/* Nothing to do. */
}

PObject Painter_clone(PObject self, PObject dst) {
	/* TODO. */
}

BOOL Painter_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Painter_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD Painter_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Painter");
	}

	return s_hash;
}