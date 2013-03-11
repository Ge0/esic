#ifndef _MAIN_UI_H_
#define _MAIN_UI_H_

#include <e11_ui.h>

typedef struct _MainUI {
	E11UI e11ui;
} MainUI, *PMainUI;

PMainUI MainUI_constructor(PMainUI self);

/* Virtual functions */
/* Object */
void MainUI_destructor(PObject self);


/* Widget */
DWORD MainUI_defaultProc(PWidget self, const PEvent systemEvent);

#endif /* MAIN_UI_H_ */