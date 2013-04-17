#ifndef _DOTPEN_UI_H_
#define _DOTPEN_UI_H_

#include <e11_ui.h>

typedef struct _DotpenUI *PDotpenUI;
typedef struct _DotpenUI {
	E11UI e11ui;
} DotpenUI;

PDotpenUI DotpenUI_constructor(PDotpenUI self);

/* Virtual functions */
/* Object */
void DotpenUI_destructor(PObject self);
PObject DotpenUI_clone(PObject self, PObject dst);
BOOL DotpenUI_equalsTo(PObject self, PObject dst);
DWORD DotpenUI_hash(PObject self);

/* Virtual functions Object */
/*
#define OBJECT_VFUNCTION(return_type, function_name, arguments) \
	return_type DotpenUI_##function_name##arguments;

	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
*/

void DotpenUI_paint(PWidget self, WORD base_x, WORD base_y);
DWORD DotpenUI_defaultProc(PWidget self, const PEvent event);

/* Virtual functions widget */
/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) \
	return_type DotpenUI_##function_name##arguments;

	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/

void DotpenUI_onF1(PE11UI self, void* param);
void DotpenUI_onF2(PE11UI self, void* param);
void DotpenUI_onF3(PE11UI self, void* param);
void DotpenUI_onF4(PE11UI self, void* param);
void DotpenUI_onF5(PE11UI self, void* param);
void DotpenUI_onF6(PE11UI self, void* param);
void DotpenUI_onF7(PE11UI self, void* param);
void DotpenUI_onF8(PE11UI self, void* param);
void DotpenUI_onF9(PE11UI self, void* param);
void DotpenUI_onF10(PE11UI self, void* param);
void DotpenUI_onF11(PE11UI self, void* param);
void DotpenUI_onF12(PE11UI self, void* param);


/* Virtual functions E11UI */
/*
#define E11_UI_FUNCTION(keycode) void DotpenUI_on##keycode(PE11UI, void*);
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
*/

#endif /* _DOTPEN_UI_H_ */
