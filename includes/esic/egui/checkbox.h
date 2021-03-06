#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <esic/eapi/event.h>
#include <esic/egui/widget.h>

typedef struct _CheckBox *PCheckBox;
typedef struct _CheckBox {
	Widget widget;
	BOOL is_checked;
	BOOL is_focused;
} CheckBox;

PCheckBox CheckBox_constructor(PCheckBox self);

void CheckBox_destructor(PObject self);
PObject CheckBox_clone(PObject self, PObject dst);
BOOL CheckBox_equalsTo(PObject self, PObject dst);
DWORD CheckBox_hash(PObject self);
DWORD CheckBox_type(PObject self);


void CheckBox_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y);
DWORD CheckBox_defaultProc(PWidget self, const PEvent event);
void CheckBox_handleWidgetEvent(PWidget self, PWidgetEvent widget_event);

/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type CheckBox_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/

#define CHECKBOX(x) ((PCheckBox)x)

#endif /* _CHECKBOX_H_ */
