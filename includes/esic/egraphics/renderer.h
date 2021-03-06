#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <esic/object.h>
#include <esic/eresources/raster_font.h>

typedef struct _Renderer *PRenderer;

typedef struct _vtable_Renderer {
	void (*drawLine)(PRenderer self, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD color);
	void (*drawRectangle)(PRenderer self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD color);
	void (*drawTriangle)(PRenderer self, DWORD x0, DWORD y0, DWORD x1, DWORD y1, DWORD x2, DWORD y2, DWORD filling_color, DWORD border_color);
	void (*drawBuffer)(PRenderer self, WORD x, DWORD y, DWORD width, DWORD height, BYTE bpp, void* raw_buffer);
	void (*drawPixel)(PRenderer self, DWORD x, DWORD y, DWORD color);
	void (*drawString)(PRenderer self, PRasterFont raster_font, DWORD x, DWORD y, DWORD color, const char* text);
	void (*fillRectangle)(PRenderer self, DWORD x, DWORD y, DWORD width, DWORD height, DWORD color);
} vtable_Renderer;


typedef struct _Renderer {
	Object object;
	VTABLE(Renderer);
} Renderer;

#define RENDERER(x) ((PRenderer)x)
#define RENDERER_VTABLE(x) RENDERER(x)->vtable

#endif /* _RENDERER_H_ */