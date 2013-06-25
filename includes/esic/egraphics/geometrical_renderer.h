#ifndef _GEOMETRICAL_RENDERER_H_
#define _GEOMETRICAL_RENDERER_H_

#include <esic/egraphics/renderer.h>


typedef struct _GeometricalRenderer* PGeometricalRenderer;
typedef struct _GeometricalRenderer {
	Renderer renderer; /* Inherits from renderer */
} GeometricalRenderer;

PGeometricalRenderer GeometricalRenderer_constructor(PGeometricalRenderer self);

/* Virtual functions */
/* Object */
void GeometricalRenderer_destructor(PObject self);
PObject GeometricalRenderer_clone(PObject self, PObject dst);
BOOL GeometricalRenderer_equalsTo(PObject self, PObject dst);
DWORD GeometricalRenderer_hash(PObject self);
DWORD GeometricalRenderer_type(PObject self);

/* Renderer */
void GeometricalRenderer_drawLine(PRenderer self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);
void GeometricalRenderer_drawRectangle(PRenderer self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD background_color, DWORD border_color);
void GeometricalRenderer_drawTriangle(PRenderer self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color);
void GeometricalRenderer_drawBuffer(PRenderer self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer);
void GeometricalRenderer_drawString(PRenderer self, PRasterFont raster_font, DWORD x, DWORD y , DWORD color, const char* string);
void GeometricalRenderer_drawPixel(PRenderer self, DWORD x , DWORD y, DWORD color);
#endif /* _GEOMETRICAL_RENDERER_H_ */