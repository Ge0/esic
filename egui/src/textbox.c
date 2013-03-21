/**
 * \file textbox.c
 */
#include <esic/eapi/system.h>
#include <esic/egui/textbox.h>
#include <esic/egui/default_widget_renderer.h>

/*
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
*/

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) TextBox_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Widget) {
#define WIDGET_VFUNCTION(return_type, function_name, arguments) TextBox_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
};


/* Private functions */
void _update_carret_position(PTextBox self, Keycode code);
void _update_offset_text_position(PTextBox self);

PTextBox TextBox_constructor(PTextBox self) {

	/* Calling parent constructor */
	Widget_constructor(&self->widget);


	/* Filling vtables */
	self->widget.object.vtable = VTABLE_POINTER(Object);
	self->widget.vtable = VTABLE_POINTER(Widget);

	/* Filling members */
	self->widget.object.size = sizeof(TextBox);

	/* Constructing members */
	ZString_constructor(&self->text, "");

	/* Default properties */
	self->widget.is_focusable = 1;
	self->is_focused          = 0;
	self->text_offset         = 0;

	return self;
}

void TextBox_destructor(PObject self) {
	PTextBox real_self = (PTextBox)self;

	/* Calling parent destructor */
	Widget_destructor(self);

	/* Destructing members */
	ZString_destructor(&real_self->text.object);
}

PObject TextBox_clone(PObject self, PObject dst) {
	PTextBox real_self = (PTextBox)self;
	PTextBox real_dst  = (PTextBox)dst;

	/* Calling parent function */
	Widget_clone(self, dst);

	/* Copying members */
	ZString_clone(&real_self->text.object, &real_dst->text.object);

	return self;
}

BOOL TextBox_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD TextBox_hash(PObject self) {
	/* TODO. */
	return 0;
}

void TextBox_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	GetDefaultWidgetRenderer()->vtable->paintTextBox(GetDefaultWidgetRenderer(), (PTextBox)self, base_x, base_y);
}

void TextBox_appendChar(PTextBox self, char ch) {
	ZString_insertCharAt(&self->text, self->carret_position, ch);
	//ZString_append(&self->text, ch);
	++self->carret_position;
	_update_offset_text_position(self);
}

void TextBox_removeLastChar(PTextBox self) {
	if(self->text.size > 0) {
		ZString_removeLastChar(&self->text);
		--self->carret_position;
		_update_offset_text_position(self);
	}
}

void TextBox_removeCharAt(PTextBox self, DWORD pos) {
	if(pos >= 0 && pos < self->text.size) {
		ZString_removeCharAt(&self->text, pos);
		--self->carret_position;
		_update_offset_text_position(self);
	}
}

void TextBox_setText(PTextBox self, const char* text) {
	ZString_setData(&self->text, text);
	self->carret_position = self->text.size;
}

DWORD TextBox_defaultProc(PWidget self, const PEvent system_event) {
	PTextBox real_self = (PTextBox)self;
	DWORD request_paint = 0;
	Event custom_event;
	Event_constructor(&custom_event);

	switch(system_event->type) {

	case EVENT_BLUR:
		/* Do not draw the carret anymore */
		real_self->is_focused  = 0;

		/* Repaint the widget without the carret */
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		EsicPushEvent(&custom_event);
		break;

	case EVENT_FOCUS:
		/* Draw the carret */
		real_self->is_focused  = 1;

		/* Repaint the widget */
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
		EsicPushEvent(&custom_event);
		break;

	/*
	case EVENT_PAINT:
		TextBox_paint(self, self->parent->x, self->parent->y);
		break;
	*/

	case EVENT_KEYBOARD_KDOWN:
		/* Append the content of the textbox */

		/* From A to Z */
		if(system_event->real_event.keyboard_event.code >= KEY_a && 
			system_event->real_event.keyboard_event.code <= KEY_z) {
			BYTE key = system_event->real_event.keyboard_event.code;
			//const BYTE* keyboard = singleton_system()->vtable->getKeyState(singleton_system());
			const BYTE* keyboard = EsicGetKeyboardState();
			if(keyboard[KEY_RSHIFT] || keyboard[KEY_LSHIFT]) {
				key &= 0xDF; /* Switch to uppercase if shift is pressed */
			}
			TextBox_appendChar(real_self, key);
			request_paint = 1;

		/* Other printable characters... */
		} else if(system_event->real_event.keyboard_event.code >= KEY_SPACE &&
			system_event->real_event.keyboard_event.code <= KEY_AT) {
			TextBox_appendChar(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;

		/* Backspace (char removal) */
		} else if(system_event->real_event.keyboard_event.code == KEY_BACKSPACE) {
			if(real_self->carret_position > 0) {
				TextBox_removeCharAt(real_self, real_self->carret_position-1);
				request_paint = 1;
			}
		

		/* Suppr key (still char removal)) */
		} else if(system_event->real_event.keyboard_event.code == KEY_DELETE) {
			if(real_self->carret_position < real_self->text.size) {
				ZString_removeCharAt(&real_self->text, real_self->carret_position);
				request_paint = 1;
			}


		/* Arrows left / right in order to move carret position */
		} else if(system_event->real_event.keyboard_event.code >= KEY_RIGHT &&
			system_event->real_event.keyboard_event.code <= KEY_LEFT) {
			_update_carret_position(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;
		}

		if(request_paint) {
			custom_event.type = EVENT_PAINT;
			custom_event.real_event.widget_event.id = self->id;

			//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
			EsicPushEvent(&custom_event);
		}
		break;

	case EVENT_TIMER:
		/* Test */
		if(system_event->real_event.timer_event.id == 1) {
			real_self->draw_carret = !real_self->draw_carret;

			custom_event.type = EVENT_PAINT;
			custom_event.real_event.widget_event.id = self->id;

			//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
			EsicPushEvent(&custom_event);
		}
		break;

	default:
		return Widget_defaultProc(self, system_event);
	}

	Event_destructor(&custom_event.object);

	return 0;
}

void _update_carret_position(PTextBox self, Keycode code) {
	switch(code) {
	case KEY_LEFT:
		if(self->carret_position > 0) {
			--self->carret_position;
			/* Offset recomputing */
			_update_offset_text_position(self);
		}
		break;
	case KEY_RIGHT:
		if(self->carret_position < self->text.size) {
			++self->carret_position;
			/* Offset recomputing */
			_update_offset_text_position(self);
		}
		break;
	}
}

void _update_offset_text_position(PTextBox self) {
	WORD size_in_letters = self->widget.width / 6; /* Remove the '6'... */

	if(self->carret_position - self->text_offset > size_in_letters
		&& size_in_letters < self->text.size) {
		self->text_offset = self->carret_position - size_in_letters;
		return;
	}

	if(self->carret_position < self->text_offset) {
		self->text_offset = self->carret_position;
	}
}