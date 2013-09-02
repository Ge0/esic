#ifndef _MAIN_UI_H_
#define _MAIN_UI_H_

#include <e11_ui.h>
#include <esic/egui/graphicsview/graphics_scene.h>

/* WIDGET IDS */
#define MAIN_UI_ID_CANVAS	5

typedef struct _MainUI {
	E11UI e11ui;

} MainUI, *PMainUI;

PMainUI MainUI_constructor(PMainUI self);

/* Virtual functions */
/* Object */
void MainUI_destructor(PObject self);


/* Widget */
DWORD MainUI_defaultProc(PWidget self, const PEvent systemEvent);

#define E11_UI_FUNCTION(keycode) void MainUI_on##keycode(PE11UI);
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION

#define E11_MAINUI(x) ((PMainUI)x)

#endif /* MAIN_UI_H_ */