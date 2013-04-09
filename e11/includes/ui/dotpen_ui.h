#ifndef _DOTPEN_UI_H_
#define _DOTPEN_UI_H_

#include <e11_ui.h>

CLASS(DotpenUI) {
	E11UI e11ui;
};

PDotpenUI DotpenUI_constructor(PDotpenUI self);

/* Virtual functions Object */
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type DotpenUI_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION

/* Virtual functions widget */
#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type DotpenUI_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION

/* Virtual functions E11UI */
#define E11_UI_FUNCTION(keycode) void DotpenUI_on##keycode(PE11UI, void*);
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION

#endif /* _DOTPEN_UI_H_ */