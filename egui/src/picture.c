/**
 * \file picture.c
 */
#include <esic/egui/picture.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_vtable_object = {
	Picture_destructor,
	Picture_clone,
	NULL,
	NULL
};

static const vtable_Widget s_vtable_widget = {
	NULL,
	Picture_paint
};

PPicture Picture_constructor(PPicture self) {
	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* Fill information */
	self->widget.object.size = sizeof(Picture);
	self->widget.object.vtable = &s_vtable_object;
	self->widget.vtable        = &s_vtable_widget;

	self->icon = NULL;
}

void Picture_destructor(PObject self) {
	/* TODO. */
}

PObject Picture_clone(PObject self, PObject dst) {
	/* TODO. */
}

void Picture_paint(PWidget self, WORD base_x, WORD base_y) {
	GetDefaultWidgetRenderer()->vtable->paintPicture(GetDefaultWidgetRenderer(), (PPicture)self, base_x, base_y);
}