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

	self->renderer = NULL;

	return self;
}

void Painter_destructor(PObject self) {
	if(((PPainter)(self))->renderer != NULL) {
		DELETE(PAINTER(self)->renderer);
	}

	PAINTER(self)->renderer = NULL;
}

PObject Painter_clone(PObject self, PObject dst) {
	dst->vtable = self->vtable;
	dst->size   = self->size;

	PAINTER(dst)->clip     = PAINTER(self)->clip;
	PAINTER(dst)->color    = PAINTER(self)->color;
	PAINTER(dst)->font     = PAINTER(self)->font;
	
	PAINTER(dst)->renderer = (PRenderer)SicAlloc(PAINTER(self)->renderer->object.size);
	OBJECT_VTABLE(PAINTER(self)->renderer)->clone(OBJECT(PAINTER(self)->renderer), OBJECT(PAINTER(dst)->renderer));

	return dst;
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

void Painter_drawLine(PPainter self, DWORD x1, DWORD y1, DWORD x2, DWORD y2) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawLine(
			self->renderer,
			x1,
			y1,
			x2,
			y2,
			self->color
		);
	}
}

void Painter_drawRectangle(PPainter self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD border_color) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawRectangle(
			self->renderer,
			x,
			y,
			width,
			height,
			self->color,
			border_color
		);
	}
}
void Painter_drawTriangle(PPainter self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD border_color) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawTriangle(
			self->renderer,
			x0,
			y0,
			x1,
			y1,
			x2,
			y2,
			self->color,
			border_color
		);
	}
}

void Painter_drawBuffer(PPainter self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawBuffer(
			self->renderer,
			x,
			y,
			width,
			height,
			bpp,
			raw_buffer
		);
	}
}

void Painter_drawPixel(PPainter self, DWORD x ,DWORD y) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawPixel(
			self->renderer,
			x,
			y,
			self->color
		);
	}
}

void Painter_drawString(PPainter self, DWORD x, DWORD y, const char* text) {
	if(self->renderer != NULL) {
		RENDERER_VTABLE(self->renderer)->drawString(
			self->renderer,
			self->font,
			x,
			y,
			self->color,
			text
		);
	}
}