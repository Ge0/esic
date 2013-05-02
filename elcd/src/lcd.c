/**
 * \file lcd.c
 */
#include <esic/elcd/lcd.h>
//#include <esic/eapi/raster_font.h>
#include <esic/etools/vector.h>
//#include <assert.h>
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

/* Pointer to the accessible frame buffer, in read-only mode */
const WORD* g_lcd_frame_buffer = NULL;

/* Pointer to current font */
//PRasterFont s_current_font = NULL;

/* GR UPDATE */
static Lcd s_lcd;


/* Private functions */
/**
 * internal function, you don't need to deal with it
 */
float* _linear_interpolation(WORD t0, float f0, WORD t1, float f1);
void _LcdFill2BytesPerPixel(WORD color);

/* Internal functions for triangles */
void _lcd_fill_bottom_flat_triangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);
void _lcd_fill_top_flat_triangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);

/* End Private functions */


void _LcdInit(DWORD width, DWORD height, BYTE bpp, void* framebuffer) {
	s_lcd.width           = width;
	s_lcd.height          = height;
	s_lcd.bytes_per_pixel = bpp/8;
	s_lcd.framebuffer     = framebuffer;

	// Test
	//memset(s_lcd.framebuffer, 0xffff, s_lcd.width * s_lcd.height * s_lcd.bytes_per_pixel);
}

void LcdDrawRectangle(DWORD x, DWORD y, DWORD width, DWORD height, DWORD filling_color, DWORD border_color) {
	/* Firstly: draw the rectangle */
	/* TODO. */
	DWORD i;
	for(i = 0; i < height; i++) {
		LcdDrawLine(x, y + i, x + width, y + i, filling_color);
	}

	/* Secondly, draw the border */
	LcdDrawLine(x, y, x + width, y, border_color);						/* top one */
	LcdDrawLine(x + width, y, x + width, y + height, border_color);	/* right one */
	LcdDrawLine(x + width, y + height, x, y + height, border_color);	/* bottom one */
	LcdDrawLine(x, y + height, x, y, border_color);					/* left one */
}

void LcdFill(DWORD color) {
	switch(s_lcd.bytes_per_pixel) {
	case 1:
		memset(s_lcd.framebuffer, (BYTE)color, s_lcd.width * s_lcd.height * s_lcd.bytes_per_pixel);
		break;
	case 2:
		//memset(s_lcd.framebuffer, (WORD)color, s_lcd.width * s_lcd.height * s_lcd.bytes_per_pixel);
		_LcdFill2BytesPerPixel(LOWORD(color));
		break;
	case 3:
		/* TODO. */
		break;
	case 4:
		/* TODO. */
		break;
	}
}

void _LcdFill2BytesPerPixel(WORD color) {
	DWORD i, j;
	WORD* framebuff = (WORD*)s_lcd.framebuffer;
	for(j = 0; j < s_lcd.height; ++j) {
		for(i = 0; i < s_lcd.width; ++i) {
			framebuff[i * s_lcd.height + j] = color;
		}
	}
}

void _lcd_fill_bottom_flat_triangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	float invslope1 = (x1 - x0) / (float)(y1 - y0);
	float invslope2 = (x2 - x0) / (float)(y2 - y0);
	DWORD curx1 = x0;
	DWORD curx2 = x0;
	DWORD scanline_y = y0;

	for (; scanline_y <= y1; scanline_y++) {
		LcdDrawLine((DWORD)curx1, scanline_y, (DWORD)curx2, scanline_y, color);
		curx1 += (DWORD)invslope1;
		curx2 += (DWORD)invslope2;
	}
}


void _lcd_fill_top_flat_triangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
	double invslope1 = (x2 - x0) / (double)(y2 - y0);
	double invslope2 = (x2 - x1) / (double)(y2 - y1);
	DWORD curx1 = x2;
	DWORD curx2 = x2;
	DWORD scanline_y = y2;

	for (; scanline_y > y0; scanline_y--) {
		curx1 -= (DWORD)invslope1;
		curx2 -= (DWORD)invslope2;
		LcdDrawLine((DWORD)curx1, scanline_y, (DWORD)curx2, scanline_y, color);
	}
}

void LcdDrawTriangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color) {
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
		DWORD x3 = (int)(x0 + ((float)(y1 - y0) / (float)(y2 - y0)) * (x2 - x0));
		DWORD y3 = y1;
	
		_lcd_fill_bottom_flat_triangle(x0, y0, x1, y1, x3, y3, filling_color);
		_lcd_fill_top_flat_triangle(x1, y1, x3, y3, x2, y2, filling_color);

	}

	/* Drawing edges */
	LcdDrawLine(x0, y0, x1, y1, border_color);
	LcdDrawLine(x1, y1, x2, y2, border_color);
	LcdDrawLine(x2, y2, x0, y0, border_color);

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

void LcdDrawBuffer(DWORD x, DWORD y, DWORD width, DWORD height, void* raw_buffer) {
	DWORD i, j;

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width; ++i) {
			LcdSetPixel(x+i, y+j, (DWORD)*((BYTE*)raw_buffer + ((j * width + i) * s_lcd.bytes_per_pixel)));
		}
	}
}

/* GR Update */

void LcdSetPixel(DWORD x, DWORD y, DWORD color) {
	switch(s_lcd.bytes_per_pixel) {
	case 1:	/* 8 bpp */
		*((BYTE*)s_lcd.framebuffer + y * s_lcd.width + x) = (BYTE)color;
		break;
	case 2: /* 16 bpp */
		*((WORD*)s_lcd.framebuffer + y * s_lcd.width + x) = (WORD)color;
		break;
	case 3:
		/* TODO. */
		break;
	case 4:
		/* TODO. */
		break;
	}
}

void LcdDrawLine(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color) {
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
			LcdSetPixel(y, x, color);
		} else {
			LcdSetPixel(x, y, color);
		}


		error -= deltay;

		if(error < 0) {
			y += ystep;
			error += deltax;
		}
	}
	
}
