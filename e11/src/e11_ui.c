#include "e11_ui.h"

static const vtable_Object s_object_vtable = {
	E11UI_destructor,
	E11UI_clone,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	E11UI_defaultProc,
	E11UI_paint
};


PE11UI E11UI_constructor(PE11UI self) {


	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;
	return self;
}

void E11UI_destructor(PObject self) {
	/* Call parent destructor */
	Widget_destructor(self);
}

PObject E11UI_clone(PObject self, PObject dst) {
	/* TODO. */

	/* Call parent clone */
	Widget_clone(self, dst);
	return dst;
}

DWORD E11UI_defaultProc(PWidget self, const PEvent system_event) {
	/* TODO. */
	switch(system_event->type) {
	
	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void E11UI_paint(PWidget self, WORD base_x, WORD base_y) {
	PE11UI real_self = (PE11UI)self;
	DWORD i;
	/* Call parent paint */
	Widget_paint(self, base_x, base_y);

	/* Draw icons */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		/* TODO. */
	}
}

PRasterIcon E11UI_getIcon(PE11UI self, DWORD index) {
	if(index >= 0 && index < NUMBER_OF_ICONS) {
		return self->icons[index];
	} else {
		return NULL;
	}
}

PRasterIcon E11UI_setIcon(PE11UI self, DWORD index, PRasterIcon icon) {
	if(index >= 0 && index < NUMBER_OF_ICONS) {
		self->icons[index] = icon;
	}
}
