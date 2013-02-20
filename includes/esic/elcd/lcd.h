#ifndef _LCD_H_
#define _LCD_H_

#include <esic/esic.h>
#include <esic/elcd/lcd_font.h>

/* Flags? */
#define	LCD_DOUBLEBUF	0x01

/* Drawing modes */
#define LCD_XOR			0x00
#define LCD_OVER		0x01
#define LCD_DEL			0x02

/* Macros (USE IT CAREFULLY!) */
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define GET_BIT_STRING_BIGENDIAN(string, i) ((string[i/8] >> (7-(i%8)) & 0x01))
#define SWAP(x,y) \
		x ^= y; \
		y ^= x; \
		x ^= y;

#define TO_INDEX(x,y,width,height) (((y * width) % (width*height)) + (x % width))

typedef enum _BPP {
	BPP1=0x01,
	BPP8,
	BPP16,
	BPP24
} BPP, *PBPP;

typedef struct{
	unsigned char R;
	unsigned char V;
	unsigned char B;
}Color, *PColor; 

/**
 * initialize the lcd screen with dimensions of width*height
 * \param width: width of the screen
 * \param height: height of the screen
 * \param framebuffer: pointer to an external framebuffer
 * \param flags: flags for the lcd initialization
 */
void Lcd_init(WORD width, WORD height, void* framebuffer, DWORD flags);

/**
 * updates the lcd screen; useful if the double buffering mode is enabled,
 * otherwise no need to call it.
 */
void Lcd_update();

/**
 * Free the resources allocated by the lcd screen; you must call this function if
 * you don't need to deal with the lcd anymore in your computer program.
 */
void Lcd_destroy();

/**
 * Change the current drawing mode by the 'mode' parameter (LCD_XOR, LCD_OVER, LCD_DEL)
 * \param mode the new drawing mode
 */
void Lcd_setDrawingMode(DWORD mode);

/**
 * Clears the lcd (all pixels set to 0)
 */
void Lcd_clear();

/**
 * Draw a colored line from (x1,y1) to (x2,y2)
 */
void Lcd_drawLine(WORD x1, WORD y1, WORD x2, WORD y2, WORD color);

/**
 * Draw a rectangle which top left corner is (x,y), and left right corner is (x+width), (y+height)
 */
void Lcd_drawRectangle(WORD x, WORD y, WORD width, WORD height, WORD filling_color, WORD border_color);

/**
 * set a pixel at (x,y) to color
 */
void Lcd_setPixel(WORD x, WORD y, WORD color);

void Lcd_useFont(const char* font_name);

void Lcd_setFont(const PLcdFont plcd_font);

void Lcd_drawString(WORD x, WORD y, WORD color, const char* string);

void Lcd_drawTriangle(WORD x0, WORD y0, WORD x1, WORD y1, WORD x2, WORD y2, WORD filling_color, WORD border_color);

void Lcd_drawPicture(WORD x, WORD y, WORD width, WORD height, WORD* raw_buffer);

#endif /* _LCD_H_ */