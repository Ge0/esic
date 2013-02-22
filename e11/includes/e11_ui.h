#ifndef _E11_UI_H_
#define _E11_UI_H_

#define NUMBER_OF_ICONS 12

#include <esic/eapi/raster_icon.h>
#include <esic/egui/widget.h>

typedef struct _E11UI {
	Widget widget;
	PRasterIcon icons[NUMBER_OF_ICONS];
} E11UI, *PE11UI;

PE11UI E11UI_constructor(PE11UI self);
PRasterIcon E11UI_getIcon(PE11UI self, DWORD index);
PRasterIcon E11UI_setIcon(PE11UI self, DWORD index, PRasterIcon icon);

/* Virtual functions */
/* Object */
void E11UI_destructor(PObject self);
PObject E11UI_clone(PObject self, PObject dst);



/* Widget */
DWORD E11UI_defaultProc(PWidget self, const PEvent event);
void E11UI_paint(PWidget self, WORD base_x, WORD base_y);

#endif /* _E11_UI_H_ */