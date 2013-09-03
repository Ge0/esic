#include <esic/eapi/system.h>
#include <esic/eapi/keyboard_event.h>
#include <esic/egui/checkbox.h>
//#include <esic/egui/default_widget_renderer.h>

VTABLE_START(Object) {
	CheckBox_destructor,
	CheckBox_clone,
	CheckBox_equalsTo,
	CheckBox_hash,
	CheckBox_type
};

VTABLE_START(Widget) {
	CheckBox_defaultProc,
	CheckBox_paint,
	CheckBox_handleWidgetEvent
};

static DWORD _handle_widget_event(PWidget self, PWidgetEvent widget_event);

PCheckBox CheckBox_constructor(PCheckBox self) {
	Widget_constructor(WIDGET(self));

	OBJECT(self)->size = sizeof(CheckBox);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	WIDGET(self)->vtable = VTABLE_POINTER(Widget);


	// Widget properties
	WIDGET(self)->is_focusable = TRUE;

	// CheckBox properties
	self->is_checked = self->is_focused = FALSE;

	return self;
}

void CheckBox_destructor(PObject self) {
	Widget_destructor(self);
}

PObject CheckBox_clone(PObject self, PObject dst) {
	/* TODO. */
	Widget_clone(OBJECT(self), OBJECT(dst));
	
	dst->size   = self->size;
	dst->vtable = self->vtable;

	CHECKBOX(dst)->is_checked = CHECKBOX(self)->is_checked;
	CHECKBOX(dst)->is_focused = CHECKBOX(self)->is_focused;



	return dst;
}

BOOL CheckBox_equalsTo(PObject self, PObject dst) {
	/* TODO. */

	return FALSE;
}

DWORD CheckBox_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD CheckBox_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("CheckBox");
	}

	return s_hash;
}

DWORD CheckBox_defaultProc(PWidget self, const PEvent system_event) {
	Event custom_event;
	Keycode key_code = KEY_UNKNOWN;

	Event_constructor(&custom_event);

	switch(system_event->type) {
	case EVENT_KEYBOARD_KDOWN:
		key_code = system_event->real_event.keyboard_event.code;

		// If the user hits return or space, toggle the checkbox
		if(key_code == KEY_RETURN || key_code == KEY_SPACE) {
			CHECKBOX(self)->is_checked = (BOOL)!(CHECKBOX(self)->is_checked);

			custom_event.type = EVENT_WIDGET;
			custom_event.real_event.widget_event.type = WE_PAINT;
			custom_event.real_event.widget_event.id   = self->id;

			EsicPushEvent(&custom_event);

		}
		break;

	/*
	case EVENT_WIDGET:
		return _handle_widget_event(self, &system_event->real_event.widget_event);
		break;
	*/

	default:	
		return Widget_defaultProc(self, system_event);
	}

	Event_destructor(OBJECT(&custom_event));
	
	return 0;
}

void CheckBox_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	//GetDefaultWidgetRenderer()->vtable->paintCheckBox(GetDefaultWidgetRenderer(), (PCheckBox)self, base_x, base_y);
	/* Draw the rectangle */
	//ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
	

	//painter->color = CHECKBOX(self)->is_focused ? RGB_16B(220,220,220) : RGB_16B(180,180,180);
	Painter_fillRectangle(
		painter,
		base_x + self->x,
		base_y + self->y,
		10,
		10,
		CHECKBOX(self)->is_focused ? RGB_16B(220,220,220) : RGB_16B(180,180,180)
	);

	//painter->color = RGB_16B(0,0,0);
	Painter_drawRectangle(
		painter,
		base_x + self->x,
		base_y + self->y,
		10,
		10,
		RGB_16B(0,0,0)
	);

	/* Draw another inner rectangle if the checkbox is checked */
	painter->color = RGB_16B(0, 255, 0);
	if(CHECKBOX(self)->is_checked) {
		//ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
		painter->color = RGB_16B(0,255,0);

		Painter_fillRectangle(
			painter,
			base_x + self->x + 2,
			base_y + self->y + 2,
			6,
			6,
			RGB_16B(0,0,0)
		);
	}
}

void CheckBox_handleWidgetEvent(PWidget self, PWidgetEvent widget_event) {
	Event custom_event;
	Event_constructor(&custom_event);
	switch(widget_event->type) {

	case WE_BLUR:
		CHECKBOX(self)->is_focused = FALSE;

		/* Repaint the widget */
		custom_event.type = EVENT_WIDGET;
		custom_event.real_event.widget_event.type = WE_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	case WE_FOCUS:
		CHECKBOX(self)->is_focused = TRUE;

		custom_event.type = EVENT_WIDGET;
		custom_event.real_event.widget_event.type = WE_PAINT;
		custom_event.real_event.widget_event.id = self->id;

		EsicPushEvent(&custom_event);
		break;

	default:
		Widget_handleWidgetEvent(self, widget_event);
	}

	Event_destructor(OBJECT(&custom_event));


}