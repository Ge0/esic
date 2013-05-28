/**
 * \file lcd_painter.c
 */
#include <math.h>
#include <string.h>

#include <esic/eapi/misc.h>

#include <esic/elcd/lcd_painter.h>
#include <esic/elcd/lcd.h>

#include <esic/egraphics/triangle.h>
#include <esic/egraphics/vertice.h>

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

	return self;
}

/* Virtual functions */
/* Object*/
void LcdPainter_destructor(PObject self) {
	PLcdPainter real_self = (PLcdPainter)self;
	real_self->abstract_painter.raster_font = NULL;
}

/* AbstractPainter */
void LcdPainter_drawLine(PAbstractPainter abstract_painter, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	LcdDrawLine(x1, y1, x2, y2, color);
}
void LcdPainter_drawRectangle(PAbstractPainter abstract_painter, DWORD x, DWORD y, DWORD width, DWORD height, DWORD background_color, DWORD border_color) {
	/* Firstly: draw the rectangle */
	int i;
	for(i = 0; i < height; i++) {
		LcdDrawLine(x, y + i, x + width-1, y + i, background_color);
	}

		/* Secondly, draw the border */

	LcdDrawLine(x, y, x + width-1, y, border_color);						/* top one */
	LcdDrawLine(x + width-1, y, x + width-1, y + (height-1), border_color);	/* right one */
	LcdDrawLine(x + width-1, y + (height-1), x, y + (height-1), border_color);	/* bottom one */
	LcdDrawLine(x, y + (height-1), x, y, border_color);					/* left one */


}

void LcdPainter_drawString(PAbstractPainter self, DWORD x, DWORD y , DWORD color, const char* string) {
	PLcdPainter real_self = (PLcdPainter)self;
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
						LcdSetPixel(x_dest, y_dest, color);
					}
				}

				SicFree(character_data);

			}
		}
	}
}

void LcdPainter_drawPixel(PAbstractPainter abstract_painter, DWORD x , DWORD y , DWORD color) {
	LcdSetPixel(x, y, color);
}

void LcdPainter_drawBuffer(PAbstractPainter abstract_painter, DWORD x, DWORD y, DWORD width, DWORD height, WORD* raw_buffer) {
	DWORD i, j;

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			if(raw_buffer[j * width + i] != 0xffff) {
				LcdSetPixel(x+i, y+j, raw_buffer[j * width + i]);
			}
		}
	}
}

void LcdPainter_drawTriangle(PAbstractPainter abstract_painter, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2 , DWORD filling_color, DWORD border_color) {
	//LcdDrawTriangle(x0, y0, x1, y1, x2, y2, filling_color, border_color);
	Triangle triangle;
	

	Triangle_constructor(&triangle);
	

	triangle.v1.x = x0;
	triangle.v1.y = y0;
	triangle.v2.x = x1;
	triangle.v2.y = y1;
	triangle.v3.x = x2;
	triangle.v3.y = y2;

}

void _LcdPainter_fillFlatSideTriangleInt(PAbstractPainter abstract_painter, const PVertice v1, const PVertice v2, const PVertice v3, DWORD border_color, DWORD filling_color) {
	Vertice tmp_vertice_1;
	Vertice tmp_vertice_2;

	BOOL changed1 = FALSE;
	BOOL changed2 = FALSE;

	SDWORD e1;
	SDWORD e2;

	int i;

	SDWORD dx1 = abs((long long)v2->x - v1->x);
	SDWORD dy1 = abs((long long)v2->y - v1->y);

	SDWORD dx2 = abs((long long)v3->x - v1->x);
	SDWORD dy2 = abs((long long)v3->y - v1->y);

	SWORD signx1 = sign(v2->x - v1->x);
	SWORD signx2 = sign(v3->x - v1->x);

	SWORD signy1 = sign(v2->y - v1->y);
	SWORD signy2 = sign(v3->y - v1->y);

	
	if(dy1 > dx1) {
		/* Swap values */
		SWAP(dx1, dy1);
		changed1 = TRUE;
	}

	if(dy2 > dx2) {
		/* Swap values */
		SWAP(dx2, dy2);
		changed2 = TRUE;
	}

	e1 = 2 * dy1 - dx1;
	e2 = 2 * dy2 - dx2;


	Vertice_constructor(&tmp_vertice_1);
	Vertice_constructor(&tmp_vertice_2);

	tmp_vertice_1.x = tmp_vertice_2.x = v1->x;
	tmp_vertice_1.y = tmp_vertice_2.y = v1->y;

	for(i = 0; i <= dx1; ++i) {
		LcdDrawLine(
			tmp_vertice_1.x,
			tmp_vertice_1.y,
			tmp_vertice_2.x,
			tmp_vertice_2.y,
			filling_color
		);

		while(e1 >= 0) {
			if(changed1) {
				tmp_vertice_1.x += signx1;
			} else {
				tmp_vertice_1.y += signy1;
			}

			e1 = e1 - 2 * dx1;
		}

		if(changed1) {
			tmp_vertice_1.y += signy1;
		} else {
			tmp_vertice_1.x += signx1;
		}

		e1 = e1 + 2 * dy1;

		/* here we rendered the next point on line 1 so follow now line 2
		* until we are on the same y-value as line 1.
		*/
		while(tmp_vertice_2.y != tmp_vertice_1.y) {
			while(e2 >= 0) {
				if(changed2) {
					tmp_vertice_2.x += signx2;
				} else {
					tmp_vertice_2.y += signy2;
				}

				e2 = e2 - 2 * dx2;
			}

			if(changed2) {
				tmp_vertice_2.y = signy2;
			} else {
				tmp_vertice_2.x = signx2;
			}

			e2 = e2 + 2 * dy2;
		}
	}


}