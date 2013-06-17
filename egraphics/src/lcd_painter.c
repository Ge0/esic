/**
 * \file lcd_painter.c
 */
#include <math.h>
#include <string.h>

#include <esic/egraphics/lcd_painter.h>
#include <esic/elcd/lcd.h>
#include <esic/egraphics/triangle.h>
#include <esic/egraphics/vertice.h>

#include <esic/eresources/raster_font_factory.h>

static const vtable_Object s_object_vtable = {
	LcdPainter_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_AbstractPainter s_abstract_painter_vtable = {
	LcdPainter_drawLine,
	LcdPainter_drawRectangle,
	LcdPainter_drawString,
	LcdPainter_drawPixel,
	LcdPainter_drawBuffer,
	LcdPainter_drawTriangle
};

PLcdPainter LcdPainter_constructor(PLcdPainter self) {
	self->abstract_painter.object.size = sizeof(LcdPainter);
	self->abstract_painter.object.vtable = &s_object_vtable;
	self->abstract_painter.vtable = &s_abstract_painter_vtable;
	self->abstract_painter.raster_font = NULL;

	/* Test */
	self->abstract_painter.clipping_region.x1 = 0;
	self->abstract_painter.clipping_region.y1 = 0;
	self->abstract_painter.clipping_region.x2 = 0xFFFFFFFF;
	self->abstract_painter.clipping_region.y2 = 0xFFFFFFFF;

	return self;
}

/* Virtual functions */
/* Object*/
void LcdPainter_destructor(PObject self) {
	PLcdPainter real_self = (PLcdPainter)self;
	real_self->abstract_painter.raster_font = NULL;
}

/* AbstractPainter */
void LcdPainter_drawLine(PAbstractPainter self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	//LcdDrawLine(x1, y1, x2, y2, color);

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
			LcdPainter_drawPixel(self, y, x, color);
		} else {
			LcdPainter_drawPixel(self, x, y, color);
		}


		error -= deltay;

		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}

}
void LcdPainter_drawRectangle(PAbstractPainter self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD background_color, DWORD border_color) {
	/* Firstly: draw the rectangle */
	DWORD i;
	for(i = 0; i < height; i++) {
		LcdPainter_drawLine(self, x, y + i, x + width-1, y + i, background_color);
	}

		/* Secondly, draw the border */

	LcdPainter_drawLine(self, x, y, x + width-1, y, border_color);						/* top one */
	LcdPainter_drawLine(self, x + width-1, y, x + width-1, y + (height-1), border_color);	/* right one */
	LcdPainter_drawLine(self, x + width-1, y + (height-1), x, y + (height-1), border_color);	/* bottom one */
	LcdPainter_drawLine(self, x, y + (height-1), x, y, border_color);					/* left one */


}

void LcdPainter_drawString(PAbstractPainter self, DWORD x, DWORD y , DWORD color, const char* string, const char* font_name) {
	PLcdPainter real_self = (PLcdPainter)self;

	/* If no font provided, load 6x8 by default */
	if(font_name != NULL) {
		self->raster_font = RasterFontFactory_getRasterFont(
			font_name
		);
	} else {
		self->raster_font = RasterFontFactory_getRasterFont(
			"6x8.flcd"
		);
	}

	self->raster_font = RasterFontFactory_getRasterFont(
		font_name
	);

	if(self->raster_font != NULL) {
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

			} else {
				/* Sample ASCII */
				utf8_code = string[i];
			}

			/* Retrieve the character's data */
			character_data = RasterFont_getCharacterData(self->raster_font, utf8_code, &len, &character_width, &character_height);

			/* Data found? Map it into the framebuffer ! */
			if(character_data != NULL) {

				for(j = 0; j < len*8; j++) {
					DWORD bit = GET_BIT_STRING_BIGENDIAN(character_data, j);
					/* If the current bit is 1, switch the pixel on */
					if(bit == 1) {
						WORD x_dest = (x + i * character_width + (j % character_width));
						WORD y_dest = y + (j / character_width) /* + ((x_dest / 320) * character_height) */;
						LcdPainter_drawPixel(self, x_dest, y_dest, color);
					}
				}

				SicFree(character_data);

			}
		}
	}
}

void LcdPainter_drawPixel(PAbstractPainter self, DWORD x , DWORD y, DWORD color) {
	if(x >= self->clipping_region.x1 && y >= self->clipping_region.y1 &&
	x <= self->clipping_region.x2 && y <= self->clipping_region.y2) {
		LcdSetPixel(x, y, color);
	}
}

void LcdPainter_drawBuffer(PAbstractPainter self, DWORD x, DWORD y, DWORD width, DWORD height, WORD* raw_buffer) {
	DWORD i, j;

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			if(raw_buffer[j * width + i] != 0xffff) {
				LcdSetPixel(x+i, y+j, raw_buffer[j * width + i]);
			}
		}
	}
}

void LcdPainter_drawTriangle(PAbstractPainter self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2 , DWORD filling_color, DWORD border_color) {
	//LcdDrawTriangle(x0, y0, x1, y1, x2, y2, filling_color, border_color);
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

	Triangle_paint(&triangle.shape, self);
}

