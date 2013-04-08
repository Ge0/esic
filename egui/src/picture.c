/**
 * \file picture.c
 */
#include <esic/eapi/system.h>
#include <esic/egui/picture.h>
#include <esic/egui/default_widget_renderer.h>


static const vtable_Object s_vtable_object = {
	Picture_destructor,
	Picture_clone,
	NULL,
	NULL
};

static const vtable_Widget s_vtable_widget = {
	Picture_defaultProc,
	Picture_paint
};


/*
VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) Picture_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Widget) {
#define WIDGET_VFUNCTION(return_type, function_name, arguments) Picture_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
};
*/


/* Private */
static void _handle_widget_event(PWidget self, PWidgetEvent widget_event);

PPicture Picture_constructor(PPicture self) {
	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	self->widget.is_focusable = TRUE;

	/* Fill information */
	self->widget.object.size = sizeof(Picture);
	self->widget.object.vtable = &s_vtable_object;
	self->widget.vtable        = &s_vtable_widget;

	self->icon = NULL;
	self->is_focused = FALSE;

	return self;
}

void Picture_destructor(PObject self) {
	/* TODO. */
}

PObject Picture_clone(PObject self, PObject dst) {
	/* TODO. */

	return dst;
}

void Picture_paint(PWidget self, WORD base_x, WORD base_y) {
	GetDefaultWidgetRenderer()->vtable->paintPicture(GetDefaultWidgetRenderer(), (PPicture)self, base_x, base_y);
}

DWORD Picture_defaultProc(PWidget self, const PEvent system_event) {
	PPicture real_self = (PPicture)self;
	Event custom_event;
	Event_constructor(&custom_event);

	switch(system_event->type) {

	case EVENT_WIDGET:
		_handle_widget_event(self, &system_event->real_event.widget_event);
		break;

	/*
	case EVENT_BLUR:
		real_self->is_focused  = 0;
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		EsicPushEvent(&custom_event);
		break;
	*/

	case EVENT_KEYBOARD_KDOWN:
		/* Test: Enlight the surrounding rect? */
		//PICTURE(self)->border_color = RGB_16B(0, 128, 255);
		if(system_event->real_event.keyboard_event.code == KEY_RETURN) {
			self->is_hot = TRUE;
			custom_event.type = EVENT_WIDGET;
			custom_event.real_event.widget_event.id = self->id;
			custom_event.real_event.widget_event.type = WE_COMMAND;
			EsicPushEvent(&custom_event);

			// Repaint the widget
			custom_event.type = EVENT_PAINT;
			custom_event.real_event.widget_event.id = self->id;
			EsicPushEvent(&custom_event);
		}
		break;

	/*
	case EVENT_FOCUS:
		real_self->is_focused  = 1;

		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		EsicPushEvent(&custom_event);
		break;
	*/

	default:
		return Widget_defaultProc(self, system_event);
	}

	Event_destructor(&custom_event.object);

	return 0;
}

static void _handle_widget_event(PWidget self, PWidgetEvent widget_event) {
	Event custom_event;
	Event_constructor(&custom_event);
	switch(widget_event->type) {

	case WE_BLUR:
		/* Do not draw the carret anymore */
		PICTURE(self)->is_focused  = FALSE;
		self->is_hot = FALSE; /*To remove later maybe */

		/* Repaint the widget without the carret */
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	case WE_FOCUS:
		/* Draw the carret */
		PICTURE(self)->is_focused  = TRUE;

		/* Repaint the widget */
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	/*
	case WE_COMMAND:
		self->is_hot = TRUE;

		// Repaint the widget
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);

		break;
	*/
	}

	Event_destructor(OBJECT(&custom_event));
}