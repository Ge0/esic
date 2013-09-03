/**
 * \file label.c
 */

#include <esic/eapi/event.h>
#include <esic/egui/label.h>


VTABLE_START(Object) {
	Label_destructor,
	Label_clone,
	Label_equalsTo,
	Label_hash,
	Label_type
};

VTABLE_START(Widget) {
	Label_defaultProc,
	Label_paint,
	Widget_handleWidgetEvent
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

void Label_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	painter->color = self->color;
	//ABSTRACTPAINTER_VTABLE(self->painter)->drawString(
	Painter_drawString(
		painter,
		base_x + self->x,
		base_y + self->y,
		RGB_16B(0,0,0),
		LABEL(self)->caption.data
	);
}

DWORD Label_defaultProc(PWidget self, const PEvent system_event) {
	switch(system_event->type) {

	default:
		return Widget_defaultProc(self, system_event);
	}

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

DWORD Label_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Label");
	}

	return s_hash;
}