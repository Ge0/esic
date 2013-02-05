#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <esic/egui/widget.h>
#include <esic/etools/szstring.h>

typedef struct _Image {
	Widget widget;
	SzString text;
	WORD x;
	WORD y;
	WORD width;
	WORD height;
	char paletteColors;
	char palette;
	char bpp;
	char *rawBuffer;
} Image, *PImage;

PImage Image_constructor(PImage self);

/* Virtual functions */
/* Object */
void Image_destructor(PObject self);

/* Widget */
void Image_paint(PWidget self);

/*! \fn void createRawImageFrom2DBuffer(PImage self, const char* buffer, WORD pHeight, WORD pWidth, BPP bpp)
    \brief Create image buffer from a 2Darray
    \param self The widget
    \param buffer The 2D buffer to read into.
    \param pHeight height of the image
	\param pWidth width of the image
	\bpp   bits / pixels for the original image
	\return
*/
void createRawImageFrom2DBuffer(PImage self, 
					const char* buffer, 
					WORD pHeight, 
					WORD pWidth, 
					char bpp);

#endif /* _LABEL_H_ */