/**
 * \file lcd.h
 */
#ifndef _LCD_H_
#define _LCD_H_

#include <esic/esic.h>

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


typedef struct _Lcd {
	DWORD width;
	DWORD height;
	BYTE bytes_per_pixel;
	void* framebuffer;
} Lcd, *PLcd;


#if defined(EMULATOR)
#include <esic/elcd/emulator_lcd.h>
#define LcdInit				EmulatorLcdInit
#define	LcdDestroy			EmulatorLcdDestroy
#define LcdUpdate			EmulatorLcdUpdate
#elif defined(OPEN1788)
#include <esic/elcd/open1788_lcd.h>
#define LcdInit				Open1788LcdInit
#define LcdDestroy			Open1788LcdDestroy
#define LcdUpdate			Open1788LcdUpdate
#endif



/**
 * initialize the lcd screen with dimensions of width*height
 * \param width: width of the screen
 * \param height: height of the screen
 * \param framebuffer: pointer to an external framebuffer
 * \param flags: flags for the lcd initialization
 * \param background_color: background_color of the screen
 */
//void LcdInit(WORD width, WORD height, void* framebuffer, DWORD flags, WORD background_color);

/**
 * updates the lcd screen; useful if the double buffering mode is enabled,
 * otherwise no need to call it.
 */
//void LcdUpdate();

/**
 * Free the resources allocated by the lcd screen; you must call this function if
 * you don't need to deal with the lcd anymore in your computer program.
 */
//void LcdDestroy();

/**
 * Change the current drawing mode by the 'mode' parameter (LCD_XOR, LCD_OVER, LCD_DEL)
 * \param mode the new drawing mode
 */
//void Lcd_setDrawingMode(DWORD mode);

/**
 * Clears the lcd (all pixels set to 0)
 */
//void Lcd_clear();

/**
 * Draw a colored line from (x1,y1) to (x2,y2)
 */
//void LcdDrawLine(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);

/**
 * Draw a rectangle which top left corner is (x,y), and left right corner is (x+width), (y+height)
 */
void LcdDrawRectangle(DWORD x, DWORD y, DWORD width, DWORD height, DWORD filling_color, DWORD border_color);

/**
 * set a pixel at (x,y) to color
 */
//void LcdSetPixel(DWORD x, DWORD y, DWORD color);


void LcdDrawTriangle(DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color);

void LcdDrawBuffer(DWORD x, DWORD y, DWORD width, DWORD height, void* raw_buffer);


/* GR Update */
void LcdFill(DWORD color);
void LcdDrawLine(DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);
void LcdSetPixel(DWORD x, DWORD y, DWORD color);
void _LcdInit(DWORD width, DWORD height, BYTE bpp, void* framebuffer);

#endif /* _LCD_H_ */