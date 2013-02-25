#include <esic/elcd/lcd.h>
#include <esic/elcd/lcd_font.h>
#include <esic/etools/vector.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//#ifdef _DEBUG
#include <esic/eapi/heap.h>
//#endif


/* Buffers */
static WORD *s_frame_buffer = NULL;
static WORD *s_double_buf = NULL;

/* Default background color */
static DWORD s_default_background_color = RGB_16B(255,255,255);

/* Screen size */
static DWORD s_width = 0;
static DWORD s_height = 0;

/* Current drawing mode */
static DWORD s_drawing_mode = 0;

/* Flags */
static DWORD s_flags = 0;

/* Pointer to the accessible frame buffer, in read-only mode */
const WORD* g_lcd_frame_buffer = NULL;

/* Pointer to current font */
PLcdFont s_current_font = NULL;


/* Private functions */
/**
 * internal function, you don't need to deal with it
 */
void _Lcd_draw(DWORD index, WORD color);
float* _linear_interpolation(WORD t0, float f0, WORD t1, float f1);

/* Internal functions for triangles */
void _lcd_fill_bottom_flat_triangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD color);
void _lcd_fill_top_flat_triangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD color);

/* End Private functions */


void Lcd_init(WORD width, WORD height, void* framebuffer, DWORD flags, DWORD background_color) {
	DWORD size;
	DWORD i;
	s_width = width;
	s_height = height;
	s_flags = flags;

	/* Ensure the provided framebuffer does not point to NULL */
	assert(framebuffer != NULL);

	/* Calculate the size for any further requirement */
	size = s_width * s_height;

	s_frame_buffer = (WORD*)framebuffer;
	//s_frame_buffer = (WORD*)SicAlloc(size * sizeof(WORD));
	
	if(s_flags & LCD_DOUBLEBUF) {
		s_double_buf = (WORD*)SicAlloc(size * sizeof(WORD));
	} else {
		/* If there is no double buffering, simply directly write to the frame buffer */
		s_double_buf = s_frame_buffer;
	}

	//memset(s_frame_buffer, 0xff, size * sizeof(WORD));
	for(i = 0; i < size; ++i) {
		s_frame_buffer[i] = background_color;
	}
	memcpy(s_double_buf, s_frame_buffer, size * sizeof(WORD));

	/* Ensure the allocation succeeded if we use the double buffering option */
	assert(s_double_buf != NULL);

	g_lcd_frame_buffer = s_frame_buffer;
}

void Lcd_destroy() {

	if(s_flags & LCD_DOUBLEBUF) {
		SicFree(s_double_buf);
		s_double_buf = NULL;
	}
}

void Lcd_update() {
	memcpy(s_frame_buffer, s_double_buf, (s_width * s_height) * sizeof(WORD));
}

void Lcd_setDrawingMode(DWORD mode) {
	s_drawing_mode = mode;
}

void Lcd_drawRectangle(WORD x, WORD y, WORD width, WORD height, WORD filling_color, WORD border_color) {
	/* Firstly: draw the rectangle */
	/* TODO. */
	int i;
	for(i = 0; i <= height; i++) {
		Lcd_drawLine(x, y + i, x + width, y + i, filling_color);
	}

	/* Secondly, draw the border */
	Lcd_drawLine(x, y, x + width, y, border_color);						/* top one */
	Lcd_drawLine(x + width, y, x + width, y + height, border_color);	/* right one */
	Lcd_drawLine(x + width, y + height, x, y + height, border_color);	/* bottom one */
	Lcd_drawLine(x, y + height, x, y, border_color);					/* left one */
}

void Lcd_setPixel(WORD x, WORD y, WORD color) {
	//DWORD end = s_width * s_height;
	//DWORD index = ((y * s_width) % end) + (x % s_width);

	DWORD index = TO_INDEX(x,y,s_width,s_height);

	_Lcd_draw(index, color);
}

void Lcd_drawLine(WORD x1, WORD y1, WORD x2, WORD y2, WORD color) {

	WORD deltax;
	WORD deltay;
	double error;
	WORD ystep;
	WORD x;
	WORD y;
	BYTE steep;
	//DWORD end;
	
	steep = abs(y2 - y1) > abs(x2 - x1);

	if(steep) {
		SWAP(x1, y1);
		SWAP(x2, y2);
	}

	if(x1 > x2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}

	deltax = x2 - x1;
	deltay = abs(y2 - y1);
	error = deltax / 2;
	y = y1;

	if(y1 < y2) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	//end = s_width * s_height;
	for(x = x1; x <= x2; ++x) {
		DWORD index_frame_buffer;
		if(steep) {
			//index_frame_buffer = ((x * s_width) % end) + (y % s_width);
			index_frame_buffer = TO_INDEX(y,x,s_width,s_height);
		} else {
			//index_frame_buffer = ((y * s_width) % end) + (x % s_width);
			index_frame_buffer = TO_INDEX(x,y,s_width,s_height);
		}

		_Lcd_draw(index_frame_buffer, color);

		error -= deltay;

		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}
	
}

void Lcd_clear() {
	if(s_flags & LCD_DOUBLEBUF) {
		memset(s_double_buf, 0, s_width * s_height * sizeof(WORD));
	} else {
		memset(s_frame_buffer, 0, s_width * s_height * sizeof(WORD));
	}
}

void _Lcd_draw(DWORD index, WORD color) {

	/* Ensure the index is within in its expected boundaries (no memory access fault) */
	assert(index < (s_width*s_height));

	/* Draw it */
	switch(s_drawing_mode) {
	case LCD_OVER:
		s_double_buf[index] = color;
		break;

	case LCD_XOR:
		s_double_buf[index] ^= color;
		break;

	case LCD_DEL:
		s_double_buf[index] -= color;
		break;
	}
}

void Lcd_setFont(const PLcdFont plcd_font) {
	s_current_font = plcd_font;
}

void Lcd_drawString(WORD x, WORD y, WORD color, const char* string) {
	if(s_current_font != NULL) {
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
			//character_data = LcdFont_getCharacterData(s_current_font, utf8_code, &len, &character_width, &character_height);

			/* Data found? Map it into the framebuffer ! */
			if(character_data != NULL) {

				for(j = 0; j < len*8; j++) {
					DWORD bit = GET_BIT_STRING_BIGENDIAN(character_data, j);
					/* If the current bit is 1, switch the pixel on */
					if(bit == 1) {
						DWORD x_dest = (x + i * character_width + (j % character_width));
						DWORD y_dest = y + (j / character_width) + ((x_dest / s_width) * character_height);
						x_dest %= s_width;
						_Lcd_draw( TO_INDEX(x_dest, y_dest, s_width, s_height), color);
					}
				}

				SicFree(character_data);

			}
		}
	}
}

void _lcd_fill_bottom_flat_triangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD color) {
	float invslope1 = (x1 - x0) / (float)(y1 - y0);
	float invslope2 = (x2 - x0) / (float)(y2 - y0);
	float curx1 = x0;
	float curx2 = x0;
	int scanline_y = y0;

	for (; scanline_y <= y1; scanline_y++) {
		Lcd_drawLine((int)curx1, scanline_y, (int)curx2, scanline_y, color);
		curx1 += invslope1;
		curx2 += invslope2;
	}
}

void _lcd_fill_top_flat_triangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD color) {
	float invslope1 = (x2 - x0) / (float)(y2 - y0);
	float invslope2 = (x2 - x1) / (float)(y2 - y1);
	float curx1 = x2;
	float curx2 = x2;
	int scanline_y = y2;

	for (; scanline_y > y0; scanline_y--) {
		curx1 -= invslope1;
		curx2 -= invslope2;
		Lcd_drawLine((int)curx1, scanline_y, (int)curx2, scanline_y, color);
	}
}

void Lcd_drawTriangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD filling_color, WORD border_color) {
	/*
	SWORD dx0;
	SWORD dx1;
	SWORD dx2;
	*/

	/* Sorting points */
	if(y0 > y1) {
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	if(y0 > y2) {
		SWAP(x0, x2);
		SWAP(y0, y2);
	}

	if(y1 > y2) {
		SWAP(x1, x2);
		SWAP(y1, y2);
	}
	/* Points are sorted */

	if (y1 == y2) {
		_lcd_fill_bottom_flat_triangle(x0, y0, x1, y1, x2, y2, filling_color);
		/* check for trivial case of top-flat triangle */
	} else if (y0 == y1) { 
		_lcd_fill_top_flat_triangle(x0, y0, x1, y1, x2, y2, filling_color);
	} else {
		/* general case - split the triangle in a topflat and bottom-flat one */
		WORD x3 = (int)(x0 + ((float)(y1 - y0) / (float)(y2 - y0)) * (x2 - x0));
		WORD y3 = y1;
	
		_lcd_fill_bottom_flat_triangle(x0, y0, x1, y1, x3, y3, filling_color);
		_lcd_fill_top_flat_triangle(x1, y1, x3, y3, x2, y2, filling_color);

	}

	/* Drawing edges */
	Lcd_drawLine(x0, y0, x1, y1, border_color);
	Lcd_drawLine(x1, y1, x2, y2, border_color);
	Lcd_drawLine(x2, y2, x0, y0, border_color);

}

float* _linear_interpolation(WORD t0, float f0, WORD t1, float f1) {
	SWORD difference = t1 - t0;
	WORD number_of_steps = 0;
	float* values = NULL;
	//DWORD slope;

	number_of_steps = abs(difference);

	if(number_of_steps == 0) {
		values = (float*)SicAlloc(sizeof(float));
		*values = f0;
	} else {
		float slope = (f1 - f0) / (float)number_of_steps;
		float f = f0;
		WORD i;
		values = (float*)SicAlloc(number_of_steps * sizeof(float));
		for(i = 0; i < number_of_steps; ++i) {
			values[i] = f;
			f += slope;
		}
	}
	return values;

}

void Lcd_drawPicture(WORD x, WORD y, WORD width, WORD height, WORD* raw_buffer) {
	WORD i, j;

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			Lcd_setPixel(x+i, y+j, raw_buffer[j * width + i]);
		}
	}
}