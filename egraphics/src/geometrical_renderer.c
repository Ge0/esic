#include <esic/egraphics/geometrical_renderer.h>
#include <esic/egraphics/triangle.h>

/* Test: direct access to the lcd first */
#include <esic/elcd/lcd.h>

VTABLE_START(Object) {
	GeometricalRenderer_destructor,
	GeometricalRenderer_clone,
	GeometricalRenderer_equalsTo,
	GeometricalRenderer_hash,
	GeometricalRenderer_type
};

VTABLE_START(Renderer) {
	GeometricalRenderer_drawLine,
	GeometricalRenderer_drawRectangle,
	GeometricalRenderer_drawTriangle,
	GeometricalRenderer_drawBuffer
};

PGeometricalRenderer GeometricalRenderer_constructor(PGeometricalRenderer self) {
	OBJECT(self)->size     = sizeof(GeometricalRenderer);
	OBJECT(self)->vtable   = VTABLE_POINTER(Object);
	RENDERER(self)->vtable = VTABLE_POINTER(Renderer);
	return self;
}

void GeometricalRenderer_destructor(PObject self) {
}

PObject GeometricalRenderer_clone(PObject self, PObject dst) {
	/* TODO. */
	return NULL;
}

BOOL GeometricalRenderer_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD GeometricalRenderer_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD GeometricalRenderer_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("GeometricalRenderer");
	}
	return s_hash;
}

void GeometricalRenderer_drawLine(PRenderer self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	DWORD deltax;
	DWORD deltay;
	double error;
	DWORD ystep;
	DWORD x;
	DWORD y;
	BYTE steep;
	
	steep = abs((long)y2 - (long)y1) > abs((long)x2 - (long)x1);

	if(steep) {
		SWAP(x1, y1);
		SWAP(x2, y2);
	}

	if(x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	deltax = x2 - x1;
	deltay = abs((long)y2 - (long)y1);
	error = deltax / 2;
	y = y1;

	if(y1 < y2) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for(x = x1; x <= x2; ++x) {
		if(steep) {
			//LcdPainter_drawPixel(self, y, x, color);
			LcdSetPixel(x, y, color);
		} else {
			//LcdPainter_drawPixel(self, x, y, color);
			LcdSetPixel(x, y, color);
		}


		error -= deltay;

		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}
}

void GeometricalRenderer_drawRectangle(PRenderer self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD background_color, DWORD border_color) {
	/* Firstly: draw the rectangle */
	DWORD i;
	for(i = 0; i < height; i++) {
		RENDERER_VTABLE(self)->drawLine(self, x, y + i, x + width-1, y + i, background_color);
	}

		/* Secondly, draw the border */

	RENDERER_VTABLE(self)->drawLine(self, x, y, x + width-1, y, border_color);						/* top one */
	RENDERER_VTABLE(self)->drawLine(self, x + width-1, y, x + width-1, y + (height-1), border_color);	/* right one */
	RENDERER_VTABLE(self)->drawLine(self, x + width-1, y + (height-1), x, y + (height-1), border_color);	/* bottom one */
	RENDERER_VTABLE(self)->drawLine(self, x, y + (height-1), x, y, border_color);					/* left one */
}

void GeometricalRenderer_drawTriangle(PRenderer self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color) {
	Triangle triangle;
	

	Triangle_constructor(&triangle);
	

	triangle.v1.x = x0;
	triangle.v1.y = y0;
	triangle.v2.x = x1;
	triangle.v2.y = y1;
	triangle.v3.x = x2;
	triangle.v3.y = y2;
	
	triangle.shape.color        = filling_color;
	triangle.shape.border_color = border_color;

	//Triangle_paint(SHAPE(&triangle), self);
}

void GeometricalRenderer_drawBuffer(PRenderer self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer) {

}

void GeometricalRenderer_drawString(PRenderer self, PRasterFont font, DWORD x, DWORD y , DWORD color, const char* string) {

}

void GeometricalRenderer_drawPixel(PRenderer self, DWORD x , DWORD y, DWORD color) {

}