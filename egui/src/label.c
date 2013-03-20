/**
 * \file label.c
 */
#include <esic/eapi/event.h>
#include <esic/egui/label.h>
#include <esic/egui/default_widget_renderer.h> /* Only for testing */

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

PLabel Label_constructor(PLabel self) {

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	self->widget.is_focusable = FALSE;


	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;

	/* Filling members */
	self->widget.object.size = sizeof(Label);

	/* Constructing members */
	ZString_constructor(&self->caption, "");

	return self;
}

void Label_destructor(PObject self) {
	PLabel real_self = (PLabel)self;

	/* Calling parent destructor */
	Widget_destructor(self);

	/* Destructing members */
	ZString_destructor(&real_self->caption.object);
}

PObject Label_clone(PObject self, PObject dst) {
	PLabel real_self = (PLabel)self;
	PLabel real_dst  = (PLabel)dst;

	/* Calling parent function */
	Widget_clone(self, dst);

	/* Copying members */
	ZString_clone(&real_self->caption.object, &real_dst->caption.object);

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