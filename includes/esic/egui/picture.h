/**
 * \file picture.h
 */
#ifndef _PICTURE_H_
#define _PICTURE_H_

#include <esic/egui/widget.h>
#include <esic/eapi/raster_icon.h>

typedef struct _Picture {
	Widget widget;
	WORD border_color;
	WORD border_thickness;
	WORD border_color_hot;
	BOOL is_focused;
	PRasterIcon icon;
} Picture, *PPicture;

PPicture Picture_constructor(PPicture self);

/* Virtual functions */
/* Object */
void Picture_destructor(PObject self);
PObject Picture_clone(PObject self, PObject dst);

/* Widget */
void Picture_paint(PWidget self, WORD base_x, WORD base_y);
DWORD Picture_defaultProc(PWidget self, const PEvent system_event);

#define PICTURE(x) ((PPicture)x)

#endif /* _PICTURE_H_ */