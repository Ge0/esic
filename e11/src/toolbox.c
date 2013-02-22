#include "toolbox.h"

PToolBox ToolBox_constructor(PToolBox self) {

}

PRasterIcon ToolBox_getIcon(PToolBox self, DWORD index) {
	if(index >= 0 && index < NUMBER_OF_ICONS) {
		return &self->icons[index];
	} else {
		return NULL;
	}
}