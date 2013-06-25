#include <esic/egraphics/geometrical_renderer.h>
#include <esic/egraphics/triangle.h>
#include <string.h>

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
	GeometricalRenderer_drawBuffer,
	GeometricalRenderer_drawPixel,
	GeometricalRenderer_drawString
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
	dst->size = self->size;
	dst->vtable = self->vtable;

	RENDERER(dst)->vtable = RENDERER(self)->vtable;

	return dst;
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
			LcdSetPixel(y, x, color);
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
	DWORD i, j;
	DWORD pixel = 0;
	BOOL condition;
	WORD* word_buffer = (WORD*)raw_buffer;
	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			switch(bpp) {
			case 1:
				pixel = (BYTE)*((BYTE*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xff);
				break;
			case 2:
				pixel = (WORD)*((WORD*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xffff);
				//LcdSetPixel(x+i, y+j, word_buffer[j * width + i]);
				break;

			case 4:
				pixel = *((DWORD*)raw_buffer + (j * width + i));
				condition = (BOOL)(pixel != 0xffffffff);
				break;

			default:
				condition = FALSE;
				pixel = 0;
			}
			
			//if(raw_buffer[j * width + i] != 0xffff) {
			
			if(condition) {
				LcdSetPixel(x+i, y+j, pixel);
			}
		}
	}
}

void GeometricalRenderer_drawString(PRenderer self, PRasterFont font, DWORD x, DWORD y, DWORD color, const char* string) {
	WORD i, j;
	DWORD len;
	BYTE character_width, character_height;
	DWORD size_string = strlen(string);
	char* character_data = NULL;


	/* For each character */
	for(i = 0; i < size_string; ++i) {
		int utf8_code = 0;

		/* If the code of the char is above 0x7F, then this is UTF-8) */
		if(string[i] > 0x7F) {
			/* TODO: draw unicode character */
		} else {
			/* Sample ASCII */
			utf8_code = string[i];
		}

		/* Retrieve the character's data */
		character_data = RasterFont_getCharacterData(font, utf8_code, &len, &character_width, &character_height);

		/* Data found? Map it into the framebuffer ! */
		if(character_data != NULL) {

			for(j = 0; j < len*8; j++) {
				DWORD bit = GET_BIT_STRING_BIGENDIAN(character_data, j);
				/* If the current bit is 1, switch the pixel on */
				if(bit == 1) {
					WORD x_dest = (x + i * character_width + (j % character_width));
					WORD y_dest = y + (j / character_width) /* + ((x_dest / 320) * character_height) */;
					LcdSetPixel(x_dest, y_dest, color);
				}
			}

			SicFree(character_data);

		}
	}
}

void GeometricalRenderer_drawPixel(PRenderer self, DWORD x, DWORD y, DWORD color) {

}