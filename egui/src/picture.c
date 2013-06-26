/**
 * \file picture.c
 */
#include <esic/eapi/system.h>
#include <esic/egui/picture.h>
//#include <esic/egui/default_widget_renderer.h>
#include <esic/egraphics/painter.h>


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

	/* Calling parent destructor */
	Widget_destructor(OBJECT(&PICTURE(self)->widget));
}

PObject Picture_clone(PObject self, PObject dst) {
	/* TODO. */

	return dst;
}

void Picture_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	//GetDefaultWidgetRenderer()->vtable->paintPicture(GetDefaultWidgetRenderer(), (PPicture)self, base_x, base_y);
	WORD border_color = PICTURE(self)->border_color;
	if(PICTURE(self)->is_focused) {
		//border_color ^= 0xFFFF;
		border_color = PICTURE(self)->border_color_hot;
	}
	
	if(self->is_hot) {
		border_color = PICTURE(self)->border_color_hot;
	}

	if(PICTURE(self)->icon != NULL) {
		Painter_drawBuffer(
			painter,
			base_x + PICTURE(self)->widget.x,
			/*base_y*/0 + PICTURE(self)->widget.y,
			PICTURE(self)->icon->header.width,
			PICTURE(self)->icon->header.height,
			2,
			PICTURE(self)->icon->data
		);

		/* ... & the surronding rect */

		/* Top border */
		painter->color = border_color;
		Painter_drawRectangle(
			painter,
			base_x + self->x - PICTURE(self)->border_thickness,
			self->y - PICTURE(self)->border_thickness,
			PICTURE(self)->icon->header.width + PICTURE(self)->border_thickness,
			PICTURE(self)->border_thickness,
			border_color
		);

		/* Right border */
		Painter_drawRectangle(
			painter,
			base_x + self->x + PICTURE(self)->icon->header.width,
			/*base_y*/0 + self->y - PICTURE(self)->border_thickness,
			PICTURE(self)->border_thickness,
			PICTURE(self)->icon->header.height + PICTURE(self)->border_thickness,
			border_color
		);

		/* Bottom border */
		Painter_drawRectangle(
			painter,
			base_x + self->x,
			/*base_y*/0 + self->y + PICTURE(self)->icon->header.height,
			PICTURE(self)->icon->header.width + PICTURE(self)->border_thickness,
			PICTURE(self)->border_thickness,
			border_color
		);

		/* Left border */
		Painter_drawRectangle(
			painter,
			base_x + self->x - PICTURE(self)->border_thickness,
			/*base_y*/ 0 + self->y,
			PICTURE(self)->border_thickness,
			PICTURE(self)->icon->header.height + PICTURE(self)->border_thickness,
			border_color
		);



	}
}

DWORD Picture_defaultProc(PWidget self, const PEvent system_event) {
	Event custom_event;
	Event_constructor(&custom_event);

	switch(system_event->type) {

	case EVENT_WIDGET:
		_handle_widget_event(self, &system_event->real_event.widget_event);
		break;

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
			custom_event.type = EVENT_WIDGET;
			custom_event.real_event.widget_event.type = WE_PAINT;
			custom_event.real_event.widget_event.id = self->id;
			EsicPushEvent(&custom_event);
		}
		break;

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
		custom_event.type = EVENT_WIDGET;
		custom_event.real_event.widget_event.type = WE_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	case WE_FOCUS:
		/* Draw the carret */
		PICTURE(self)->is_focused  = TRUE;

		/* Repaint the widget */
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