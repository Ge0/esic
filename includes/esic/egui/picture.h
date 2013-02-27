/**
 * \file picture.h
 */
#ifndef _PICTURE_H_
#define _PICTURE_H_

#include <esic/egui/widget.h>
#include <esic/eapi/raster_icon.h>

typedef struct _Picture {
	Widget widget;
	PRasterIcon icon;
} Picture, *PPicture;

PPicture Picture_constructor(PPicture self);

/* Virtual functions */
/* Object */
void Picture_destructor(PObject self);
PObject Picture_clone(PObject self, PObject dst);

/* Widget */
void Picture_paint(PWidget self, WORD base_x, WORD base_y);


#endif /* _PICTURE_H_ */