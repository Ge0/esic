/**
 * \file abstract_painter.h
 */
#ifndef _ABSTRACT_PAINTER_H_
#define _ABSTRACT_PAINTER_H_

#include <esic/object.h>
#include <esic/eapi/event.h>
#include <esic/eapi/raster_font.h>

typedef struct _AbstractPainter* PAbstractPainter;

typedef struct _vtable_AbstractPainter {
	void (*drawLine)(PAbstractPainter, WORD, WORD, WORD, WORD, WORD);
	void (*drawRectangle)(PAbstractPainter, WORD, WORD, WORD, WORD, WORD, WORD);
	void (*drawString)(PAbstractPainter, WORD, WORD, WORD, const char*);
	void (*drawPixel)(PAbstractPainter, WORD, WORD, WORD);
	void (*drawBuffer)(PAbstractPainter, WORD, WORD, WORD, WORD, WORD*);
} vtable_AbstractPainter;

typedef struct _AbstractPainter {
	Object object;
	const vtable_AbstractPainter* vtable;
	PRasterFont raster_font;
} AbstractPainter;


#endif /* _ABSTRACT_SYSTEM_H_ */