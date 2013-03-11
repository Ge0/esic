#ifndef _E11_UI_H_
#define _E11_UI_H_

#define E11_NUMBER_OF_FUNCTIONS		12
#define ICONS_BASE_X				9
#define ICONS_BASE_Y				164
#define ICONS_PER_LINE				6
#define E11_NUMBER_OF_ICONS			12
#define BORDER_THICKNESS			2
#define MARGIN_SECOND_ICON_LINE	39
#define BACKGROUND_FIRST_ROW RGB_16B(0,162,232)
#define BACKGROUND_SECOND_ROW RGB_16B(171,220,24)

#include <esic/egui/picture.h>
#include <esic/egui/widget.h>

typedef struct _E11UI *PE11UI;

typedef struct _E11UI {
	Widget widget;
	void (*onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI);	/* array of function pointers */
	Picture icons[E11_NUMBER_OF_ICONS];
	WORD hot_widget_id; /* Test */
	PListNode focused_widget;
} E11UI, *PE11UI;

PE11UI E11UI_constructor(PE11UI self);
PPicture E11UI_getPicture(PE11UI self, DWORD index);
void E11UI_setPicture(PE11UI self, DWORD index, const PPicture picture);

/* Virtual functions */
/* Object */
void E11UI_destructor(PObject self);
PObject E11UI_clone(PObject self, PObject dst);



/* Widget */
DWORD E11UI_defaultProc(PWidget self, const PEvent event);
void E11UI_paint(PWidget self, WORD base_x, WORD base_y);

#endif /* _E11_UI_H_ */