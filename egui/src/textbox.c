#include <esic/eapi/abstract_system.h>
#include <esic/egui/textbox.h>
#include <esic/egui/default_widget_renderer.h>

static const vtable_Object s_object_vtable = {
	TextBox_destructor,
	TextBox_clone,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	TextBox_defaultProc,
	TextBox_paint
};


/* Private functions */
void _update_carret_position(PTextBox self, Keycode code);

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

	/* Default properties */
	self->widget.is_focusable = TRUE;
	self->draw_carret         = FALSE;

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

	return self;
}

void TextBox_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	GetDefaultWidgetRenderer()->vtable->paintTextBox(GetDefaultWidgetRenderer(), (PTextBox)self, base_x, base_y);
}

void TextBox_appendChar(PTextBox self, char ch) {
	SzString_insertCharAt(&self->text, self->carret_pos, ch);
	//SzString_append(&self->text, ch);
	++self->carret_pos;
}

void TextBox_removeLastChar(PTextBox self) {
	if(self->text.size > 0) {
		SzString_removeLastChar(&self->text);
		--self->carret_pos;
	}
}

void TextBox_setText(PTextBox self, const char* text) {
	SzString_setData(&self->text, text);
	self->carret_pos = self->text.size;
}

DWORD TextBox_defaultProc(PWidget self, const PEvent system_event) {
	PTextBox real_self = (PTextBox)self;
	DWORD request_paint = 0;
	Event custom_event;

	switch(system_event->type) {
	case EVENT_PAINT:
		TextBox_paint(self, self->parent->x, self->parent->y);
		break;

	case EVENT_KEYBOARD_KDOWN:
		/* Append the content of the textbox */

		/* From A to Z */
		if(system_event->real_event.keyboard_event.code >= KEY_a && 
			system_event->real_event.keyboard_event.code <= KEY_z) {
			TextBox_appendChar(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;

		/* Other printable characters... */
		} else if(system_event->real_event.keyboard_event.code >= KEY_SPACE &&
			system_event->real_event.keyboard_event.code <= KEY_AT) {
			TextBox_appendChar(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;

		/* Backspace (char removal) */
		} else if(system_event->real_event.keyboard_event.code == KEY_BACKSPACE) {
			TextBox_removeLastChar(real_self);
			request_paint = 1;
		}

		/* Arrows left / right in order to move carret position */
		else if(system_event->real_event.keyboard_event.code >= KEY_RIGHT &&
			system_event->real_event.keyboard_event.code <= KEY_LEFT) {
			_update_carret_position(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;
		}

		if(request_paint) {
			custom_event.type = EVENT_PAINT;
			custom_event.real_event.paint_event.id = self->id;

			singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		}
		break;

	case EVENT_TIMER:
		/* Test */
		if(system_event->real_event.timer_event.id == 1) {
			real_self->draw_carret = !real_self->draw_carret;

			custom_event.type = EVENT_PAINT;
			custom_event.real_event.paint_event.id = self->id;

			singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		}
		break;

	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void _update_carret_position(PTextBox self, Keycode code) {
	switch(code) {
	case KEY_LEFT:
		if(self->carret_pos > 0) {
			--self->carret_pos;
		}
		break;
	case KEY_RIGHT:
		if(self->carret_pos < self->text.size) {
			++self->carret_pos;
		}
		break;
	}
}