/**
 * \file lcd_painter.c
 */
#include <esic/elcd/lcd_painter.h>
#include <esic/elcd/lcd.h>

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
	LcdPainter_drawBuffer
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
void LcdPainter_drawLine(PAbstractPainter abstract_painter, WORD x1, WORD y1, WORD x2, WORD y2, WORD color) {
	Lcd_drawLine(x1, y1, x2, y2, color);
}
void LcdPainter_drawRectangle(PAbstractPainter abstract_painter, WORD x, WORD y, WORD width, WORD height, WORD background_color, WORD border_color) {
	/* Firstly: draw the rectangle */
	int i;
	for(i = 0; i <= height; i++) {
		Lcd_drawLine(x, y + i, x + width, y + i, background_color);
	}

	/* Secondly, draw the border */
	Lcd_drawLine(x, y, x + width, y, border_color);						/* top one */
	Lcd_drawLine(x + width, y, x + width, y + height, border_color);	/* right one */
	Lcd_drawLine(x + width, y + height, x, y + height, border_color);	/* bottom one */
	Lcd_drawLine(x, y + height, x, y, border_color);					/* left one */
}

void LcdPainter_drawString(PAbstractPainter self, WORD x, WORD y , WORD color, const char* string) {
	PLcdPainter real_self = (PLcdPainter)self;
	if(self->raster_font != NULL) {
		DWORD i, j, len;
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
						DWORD x_dest = (x + i * character_width + (j % character_width));
						DWORD y_dest = y + (j / character_width) /* + ((x_dest / 320) * character_height) */;
						Lcd_setPixel(x_dest, y_dest, color);
					}
				}

				SicFree(character_data);

			}
		}
	}
}

void LcdPainter_drawPixel(PAbstractPainter abstract_painter, WORD x , WORD y , WORD color) {
	Lcd_setPixel(x, y, color);
}

void LcdPainter_drawBuffer(PAbstractPainter abstract_painter, WORD x, WORD y, WORD width, WORD height, WORD* raw_buffer) {
	WORD i, j;

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			Lcd_setPixel(x+i, y+j, raw_buffer[j * width + i]);
		}
	}
}