/**
 * \file lcd_painter.h
 */
#ifndef _LCD_PAINTER_H_
#define _LCD_PAINTER_H_

#include <esic/eapi/abstract_painter.h>

typedef struct _LcdPainter {
	AbstractPainter abstract_painter;
} LcdPainter, *PLcdPainter;

PLcdPainter LcdPainter_constructor(PLcdPainter self);

/* Virtual functions */
/* Object*/
void LcdPainter_destructor(PObject self);

/* AbstractPainter */
void LcdPainter_drawLine(PAbstractPainter, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);
void LcdPainter_drawRectangle(PAbstractPainter, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD background_color, DWORD border_color);
void LcdPainter_drawString(PAbstractPainter, DWORD, DWORD, DWORD, const char*);
void LcdPainter_drawPixel(PAbstractPainter, DWORD, DWORD, DWORD);
void LcdPainter_drawBuffer(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, WORD*);
void LcdPainter_drawTriangle(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);

#endif /* _LCD_PAINTER_H_ */