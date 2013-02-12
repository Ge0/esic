#include <esic/egui/textbox.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_object_vtable = {
	TextBox_destructor,
	TextBox_clone,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	Widget_defaultProc,
	TextBox_paint
};



PTextBox TextBox_constructor(PTextBox self) {

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable = &s_widget_vtable;

	/* Filling members */
	self->widget.object.size = sizeof(TextBox);

	/* Constructing members */
	SzString_constructor(&self->text, "");

	return self;
}

void TextBox_destructor(PObject self) {
	PTextBox real_self = (PTextBox)self;

	/* Calling parent destructor */
	Widget_destructor(self);

	/* Destructing members */
	SzString_destructor(&real_self->text.object);
}

PObject TextBox_clone(PObject self, PObject dst) {
	PTextBox real_self = (PTextBox)self;
	PTextBox real_dst  = (PTextBox)dst;

	/* Calling parent function */
	Widget_clone(self, dst);

	/* Copying members */
	SzString_clone(&real_self->text.object, &real_dst->text.object);

}

void TextBox_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	DefaultWidgetRenderer_paintTextBox((PTextBox)self, base_x, base_y);
}

void TextBox_setText(PTextBox self, const char* text) {
	SzString_setData(&self->text, text);
}