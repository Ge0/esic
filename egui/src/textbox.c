/**
 * \file textbox.c
 */
#include <esic/eapi/system.h>
#include <esic/egui/textbox.h>
//#include <esic/egui/default_widget_renderer.h>
#include <esic/eresources/raster_font_factory.h>

#include <string.h>

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
/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) TextBox_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/
	TextBox_defaultProc,
	TextBox_paint,
	TextBox_handleWidgetEvent
};


/* Private functions */
static void _update_carret_position(PTextBox self, Keycode code);
static void _update_offset_text_position(PTextBox self);

PTextBox TextBox_constructor(PTextBox self) {

	/* Calling parent constructor */
	Widget_constructor(&self->widget);


	/* Filling vtables */
	self->widget.object.vtable = VTABLE_POINTER(Object);
	self->widget.vtable = VTABLE_POINTER(Widget);

	/* Filling members */
	self->widget.object.size = sizeof(TextBox);

	/* Constructing members */
	ZStringBuffer_constructor(&self->text);
	//ZString_constructor(&self->text, "");

	/* Default properties */
	self->widget.is_focusable = TRUE;
	self->is_focused          = FALSE;
	self->text_offset         = 0;
	self->draw_carret		  = FALSE;

	return self;
}

void TextBox_destructor(PObject self) {
	PTextBox real_self = (PTextBox)self;

	/* Calling parent destructor */
	Widget_destructor(self);

	/* Destructing members */
	//ZString_destructor(&real_self->text.object);
	ZStringBuffer_destructor(&real_self->text.object);
}

PObject TextBox_clone(PObject self, PObject dst) {
	PTextBox real_self = (PTextBox)self;
	PTextBox real_dst  = (PTextBox)dst;

	/* Calling parent function */
	Widget_clone(self, dst);

	/* Copying members */
	//ZString_clone(&real_self->text.object, &real_dst->text.object);
	ZStringBuffer_clone(&real_self->text.object, &real_dst->text.object);

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

DWORD TextBox_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("TextBox");
	}

	return s_hash;
}

void TextBox_paint(PWidget self, WORD base_x, WORD base_y) {
	/* TODO */

	/* TEST */
	//GetDefaultWidgetRenderer()->vtable->paintTextBox(GetDefaultWidgetRenderer(), (PTextBox)self, base_x, base_y);

	//ZString visible_text;
	ZStringBuffer visible_text;
	WORD character_width = 0;
	PRasterFont font6x8 = NULL;

	font6x8 = RasterFontFactory_getRasterFont("6x8.flcd"); /* Font to define dynamically later? */
	if(font6x8 != NULL) {
		character_width = font6x8->header.base_character_width;
	}

	/* Draw the surrounding rect with border */
	ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
		self->painter,
		base_x + self->x,
		base_y + self->y,
		self->width,
		self->height + 4,
		TEXTBOX(self)->is_focused ? TEXTBOX(self)->focused_background_color : TEXTBOX(self)->background_color,
		TEXTBOX(self)->is_focused ? TEXTBOX(self)->focused_border_color     : TEXTBOX(self)->border_color
	);
	
	/* Draw the only visible part of text */
	//ZString_constructor(&visible_text, "");
	ZStringBuffer_constructor(&visible_text);

	//ZString_subString(&textbox->text, textbox->text_offset, textbox->widget.width / character_width, &visible_text);
	ZStringBuffer_subString(
		&TEXTBOX(self)->text,
		TEXTBOX(self)->text_offset,
		TEXTBOX(self)->widget.width / character_width,
		&visible_text
	); 

	/* Draw the string */
	ABSTRACTPAINTER_VTABLE(self->painter)->drawString(
		self->painter,
		base_x + TEXTBOX(self)->widget.x + 2,
		base_y + TEXTBOX(self)->widget.y + 2,
		RGB_16B(0,0,0),
		visible_text.data,
		"6x8.flcd"
	);

	if(TEXTBOX(self)->draw_carret && TEXTBOX(self)->is_focused) {
		/* Draw the carret */
		ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
			self->painter,
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + TEXTBOX(self)->widget.x + 2 + ((TEXTBOX(self)->carret_position - TEXTBOX(self)->text_offset) * character_width) ,
			base_y + TEXTBOX(self)->widget.y + 2,
			1,
			TEXTBOX(self)->widget.height,
			RGB_16B(0,0,0),
			RGB_16B(0,0,0)
		);
	}

	/* Draw indicators at the beginning if there are characters non printed */
	if(TEXTBOX(self)->text_offset) {
		ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
			self->painter,
			base_x + TEXTBOX(self)->widget.x ,
			base_y + TEXTBOX(self)->widget.y,
			2,
			TEXTBOX(self)->widget.height+4,
			RGB_16B(20,20,20),
			RGB_16B(20,20,20)
		);
	}

	//if((WORD)(textbox->text_offset + (textbox->widget.width / character_width)) < textbox->text.size) {
	if((WORD)(TEXTBOX(self)->text_offset + (TEXTBOX(self)->widget.width / character_width)) < TEXTBOX(self)->text.logical_size) {
		ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
			self->painter,
			base_x + TEXTBOX(self)->widget.x + TEXTBOX(self)->widget.width-2 ,
			base_y + TEXTBOX(self)->widget.y,
			2,
			TEXTBOX(self)->widget.height+4,
			RGB_16B(20,20,20),
			RGB_16B(20,20,20)
		);
	}

	//ZString_destructor(OBJECT(&visible_text));
	ZStringBuffer_destructor(OBJECT(&visible_text));
}

void TextBox_appendChar(PTextBox self, char ch) {
	if(self->text.logical_size < TEXTBOX_DEFAULT_MAXLENGTH) {
		//ZString_insertCharAt(&self->text, self->carret_position, ch);
		ZStringBuffer_insertCharAt(&self->text, self->carret_position, ch);
		++self->carret_position;
		_update_offset_text_position(self);
	}
}

void TextBox_removeLastChar(PTextBox self) {
	//if(self->text.size > 0) {
	//	ZString_removeLastChar(&self->text);
	if(self->text.logical_size > 0) {
		ZStringBuffer_removeLastChar(&self->text);
		--self->carret_position;
		_update_offset_text_position(self);
	}
}

void TextBox_removeCharAt(PTextBox self, DWORD pos) {
	//if(/*pos >= 0 && */pos < self->text.size) {
	//	ZString_removeCharAt(&self->text, pos);
	if(pos >= 0 && pos < self->text.logical_size) {
		ZStringBuffer_removeCharAt(&self->text, pos);
		--self->carret_position;
		_update_offset_text_position(self);
	}
}

void TextBox_setText(PTextBox self, const char* text) {
	if(strlen(text) < TEXTBOX_DEFAULT_MAXLENGTH) {
		//ZString_setData(&self->text, text);
		//self->carret_position = self->text.size;
		ZStringBuffer_setData(&self->text, text);
		self->carret_position = self->text.logical_size;
	}
}

DWORD TextBox_defaultProc(PWidget self, const PEvent system_event) {
	PTextBox real_self = (PTextBox)self;
	DWORD request_paint = 0;
	Event custom_event;
	Event_constructor(&custom_event);

	switch(system_event->type) {

	case EVENT_KEYBOARD_KDOWN:
		/* Append the content of the textbox */

		/* From A to Z */
		if(system_event->real_event.keyboard_event.code >= KEY_a && 
			system_event->real_event.keyboard_event.code <= KEY_z) {
			const BYTE* keyboard = NULL;
			BYTE key = system_event->real_event.keyboard_event.code;
			//const BYTE* keyboard = singleton_system()->vtable->getKeyState(singleton_system());
			keyboard = (const BYTE*)EsicGetKeyboardState();
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
			//if(real_self->carret_position < real_self->text.size) {
			//	ZString_removeCharAt(&real_self->text, real_self->carret_position);
			if(real_self->carret_position < real_self->text.logical_size) {
				ZStringBuffer_removeCharAt(&real_self->text, real_self->carret_position);
				request_paint = 1;
			}


		/* Arrows left / right in order to move carret position */
		} else if(system_event->real_event.keyboard_event.code >= KEY_RIGHT &&
			system_event->real_event.keyboard_event.code <= KEY_LEFT) {
			_update_carret_position(real_self, system_event->real_event.keyboard_event.code);
			request_paint = 1;
		}

		if(request_paint) {
			/*
			custom_event.type = EVENT_PAINT;
			custom_event.real_event.widget_event.id = self->id;
			*/

			// for(i = 0; i < number_of_painters; ++i) {
			custom_event.type = EVENT_WIDGET;
			custom_event.real_event.widget_event.type = WE_PAINT;
			custom_event.real_event.widget_event.id = self->id;
			//custom_event.real_event.widget_event.param = (void*)painter[i]
			// }
			EsicPushEvent(&custom_event);
		}
		break;

	case EVENT_TIMER:
		/* Test */

		/* The only timer message a textbox needs is to know
		 * whether it needs to blink its carret or not; so don't
		 * handle the event if the textbox doesn't have the focus */
		if(TEXTBOX(self)->is_focused) {
			if(system_event->real_event.timer_event.id == 1) {
				real_self->draw_carret = (BOOL)!real_self->draw_carret;

				/*
				custom_event.type = EVENT_PAINT;
				custom_event.real_event.widget_event.id = self->id;
				*/

				/* GR TEST */
				custom_event.type = EVENT_WIDGET;
				custom_event.real_event.widget_event.type = WE_PAINT;
				custom_event.real_event.widget_event.id   = self->id;

				//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
				EsicPushEvent(&custom_event);
			}
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
		//if(self->carret_position < self->text.size) {
		if(self->carret_position < self->text.logical_size) {
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
		//&& size_in_letters < self->text.size) {
		&& size_in_letters < self->text.logical_size) {
		self->text_offset = self->carret_position - size_in_letters;
		return;
	}

	if(self->carret_position < self->text_offset) {
		self->text_offset = self->carret_position;
	}
}

void TextBox_handleWidgetEvent(PWidget self, PWidgetEvent widget_event) {
	Event custom_event;
	Event_constructor(&custom_event);
	switch(widget_event->type) {

	case WE_BLUR:
		/* Do not draw the carret anymore */
		TEXTBOX(self)->is_focused = FALSE;

		/* Repaint the widget without the carret */
		/*
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		*/
		custom_event.type = EVENT_WIDGET;
		custom_event.real_event.widget_event.type = WE_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	case WE_FOCUS:
		/* Draw the carret */
		TEXTBOX(self)->is_focused = TRUE;

		/* Repaint the widget */
		/*
		custom_event.type = EVENT_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		*/
		custom_event.type = EVENT_WIDGET;
		custom_event.real_event.widget_event.type = WE_PAINT;
		custom_event.real_event.widget_event.id = self->id;
		EsicPushEvent(&custom_event);
		break;

	default:
		/* TODO: HANNDLE DEFAULT WIDGET EVENT (ex: PAINT) */
		Widget_handleWidgetEvent(self, widget_event); 
		break;
	}

	Event_destructor(OBJECT(&custom_event));


}
