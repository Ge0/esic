#ifndef _E11_UI_H_
#define _E11_UI_H_

#define E11_BASE_ID_SYSTEM_ICONS	0x8000
#define E11_NUMBER_OF_FUNCTIONS		12
#define ICONS_BASE_X				9
#define ICONS_BASE_Y				164
#define ICONS_PER_LINE				6
#define E11_NUMBER_OF_ICONS			12
#define BORDER_THICKNESS			2
#define MARGIN_SECOND_ICON_LINE		39
#define BACKGROUND_FIRST_ROW		RGB_16B(0,162,232)
#define BACKGROUND_SECOND_ROW		RGB_16B(171,220,24)
#define PICTURE_BORDER_COLOR_HOT	RGB_16B(255, 128, 0)


// Test
#define E11_EVENT_ACCEPT	0x01
#define E11_EVENT_CREATE_CHILD	0x02

#include <esic/egui/picture.h>
#include <esic/egui/widget.h>

typedef struct _E11UI *PE11UI;

#define E11_UI_FUNCTION_LIST \
	E11_UI_FUNCTION(F1) \
	E11_UI_FUNCTION(F2) \
	E11_UI_FUNCTION(F3) \
	E11_UI_FUNCTION(F4) \
	E11_UI_FUNCTION(F5) \
	E11_UI_FUNCTION(F6) \
	E11_UI_FUNCTION(F7) \
	E11_UI_FUNCTION(F8) \
	E11_UI_FUNCTION(F9) \
	E11_UI_FUNCTION(F10) \
	E11_UI_FUNCTION(F11) \
	E11_UI_FUNCTION(F12)


typedef struct _vtable_E11UI {
#define E11_UI_FUNCTION(keycode) void (*on##keycode)(PE11UI, void*);
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
} vtable_E11UI;


typedef struct _E11UI {
	Widget widget;
	//const vtable_E11UI* vtable;
	//void (*onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI, void*);	/* array of function pointers */
	void (**onFunction)(PE11UI, void*);
	//Picture icons[E11_NUMBER_OF_ICONS];
	PPicture icons[E11_NUMBER_OF_ICONS];
	WORD hot_widget_id; /* Test */
	PListNode focused_widget;
	PE11UI parent_ui;
	PE11UI child_ui;
} E11UI;

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

void E11UI_accept(PE11UI self);

#define E11UI(x) ((PE11UI)x)



#define GET_UI_FUNCTION(base_struct, index) base_struct##_onF##index

#endif /* _E11_UI_H_ */
