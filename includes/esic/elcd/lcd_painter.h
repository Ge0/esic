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
void LcdPainter_drawLine(PAbstractPainter, WORD x1, WORD y1, WORD x2, WORD y2, WORD color);
void LcdPainter_drawRectangle(PAbstractPainter, WORD x1, WORD y1, WORD x2, WORD y2, WORD background_color, WORD border_color);
void LcdPainter_drawString(PAbstractPainter, WORD, WORD, WORD, const char*);
void LcdPainter_drawPixel(PAbstractPainter, WORD, WORD, WORD);
void LcdPainter_drawBuffer(PAbstractPainter, WORD, WORD, WORD, WORD, WORD*);

#endif /* _LCD_PAINTER_H_ */