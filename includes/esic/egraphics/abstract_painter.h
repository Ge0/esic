/**
 * \file abstract_painter.h
 */
#ifndef _ABSTRACT_PAINTER_H_
#define _ABSTRACT_PAINTER_H_

#include <esic/object.h>
#include <esic/eapi/event.h>
#include <esic/eresources/raster_font.h>

/* Test */
typedef struct _ClippingRegion {
	DWORD x1, y1, x2, y2;
} ClippingRegion, *PClippingRegion;

typedef struct _AbstractPainter* PAbstractPainter;

typedef struct _vtable_AbstractPainter {
	void (*drawLine)(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, DWORD);
	void (*drawRectangle)(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
	void (*drawString)(PAbstractPainter, DWORD, DWORD, DWORD, const char*, const char*);
	void (*drawPixel)(PAbstractPainter, DWORD, DWORD, DWORD);
	void (*drawBuffer)(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, WORD*);
	void (*drawTriangle)(PAbstractPainter, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
} vtable_AbstractPainter;

typedef struct _AbstractPainter {
	Object object;
	const vtable_AbstractPainter* vtable;
	PRasterFont raster_font;
	ClippingRegion clipping_region;
} AbstractPainter;



/* Useful Macro */
#define ABSTRACTPAINTER(x)			((PAbstractPainter)x)
#define ABSTRACTPAINTER_VTABLE(x)	ABSTRACTPAINTER(x)->vtable

/* X-Macro */
#define ABSTRACTPAINTER_VIRTUAL_FUNCTIONS \
	ABSTRACTPAINTER_VFUNCTION(drawLine), \
	ABSTRACTPAINTER_VFUNCTION(drawRectangle), \
	ABSTRACTPAINTER_VFUNCTION(drawString), \
	ABSTRACTPAINTER_VFUNCTION(drawPixel), \
	ABSTRACTPAINTER_VFUNCTION(drawBuffer), \
	ABSTRACTPAINTER_VFUNCTION(drawRectangle)

#endif /* _ABSTRACT_SYSTEM_H_ */