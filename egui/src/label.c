#include <esic/egui/label.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_object_vtable = {
	Label_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	Label_paint
};

PLabel Label_constructor(PLabel self) {

	/* Filling vtables */

	/* Constructing members */
	SzString_constructor(&self->caption, "");

	return self;
}

void Label_destructor(PObject self) {
	PLabel real_self = (PLabel)self;

	/* Calling parent destructor */
	//Widget_destructor(self);

	/* Destructing members */
	SzString_destructor(&real_self->caption.object);
}

void Label_paint(PWidget self) {
	/* TODO */

	/* TEST */
	DefaultWidgetRenderer_paintLabel((PLabel)self);
}