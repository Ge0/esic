#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_

#include <esic/egui/widget.h>

CLASS(CheckBox) {
	Widget widget;
	BOOL is_checked;
	BOOL is_focused;
};

PCheckBox CheckBox_constructor(PCheckBox self);

#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type CheckBox_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type CheckBox_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION

#define CHECKBOX(x) ((PCheckBox)x)

#endif /* _CHECKBOX_H_ */