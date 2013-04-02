/**
 * \file label.c
 */

#include <esic/eapi/event.h>
#include <esic/egui/label.h>
#include <esic/egui/default_widget_renderer.h> /* Only for testing */

/*
static const vtable_Object s_object_vtable = {
	Label_destructor,
	Label_clone,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	Widget_defaultProc,
	Label_paint
};
*/

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) Label_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Widget) {
#define WIDGET_VFUNCTION(return_type, function_name, arguments) Label_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
};

PLabel Label_constructor(PLabel self) {

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	self->widget.is_focusable = FALSE;


	/* Filling vtables */
	self->widget.object.vtable = VTABLE_POINTER(Object);
	self->widget.vtable        = VTABLE_POINTER(Widget);

	/* Filling members */
	self->widget.object.size = sizeof(Label);

	/* Constructing members */
	ZString_constructor(&self->caption, "");

	return self;
}

void Label_destructor(PObject self) {

	/* Calling parent destructor */
	Widget_destructor(self);

	/* Destructing members */
	ZString_destructor(OBJECT(&LABEL(self)->caption));
}

PObject Label_clone(PObject self, PObject dst) {

	/* Calling parent function */
	Widget_clone(self, dst);

	/* Copying members */
	ZString_clone(OBJECT(&LABEL(self)->caption), OBJECT(&LABEL(dst)->caption));
	return self;
}

void Label_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	GetDefaultWidgetRenderer()->vtable->paintLabel(GetDefaultWidgetRenderer(), (PLabel)self, base_x, base_y);
}

DWORD Label_defaultProc(PWidget self, const PEvent system_event) {
	switch(system_event->type) {

	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void Label_setCaption(PLabel self, const char* text) {
	ZString_setData(&self->caption, text);
}

BOOL Label_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}


DWORD Label_hash(PObject self) {
	/* TODO. */
	return 0;
}