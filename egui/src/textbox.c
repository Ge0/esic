#include <esic/egui/textbox.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_object_vtable = {
	TextBox_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	TextBox_paint
};

PTextBox TextBox_constructor(PTextBox self) {

	/* Filling vtables */

	/* Constructing members */
	SzString_constructor(&self->text, "");

	return self;
}

void TextBox_destructor(PObject self) {
	PTextBox real_self = (PTextBox)self;

	/* Calling parent destructor */
	//Widget_destructor(self);

	/* Destructing members */
	SzString_destructor(&real_self->text.object);
}

void TextBox_paint(PWidget self) {
	/* TODO */

	/* TEST */
	DefaultWidgetRenderer_paintTextBox((PTextBox)self);
}