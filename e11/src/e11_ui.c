#include <esic/eapi/abstract_system.h>
#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/widget_ptr.h>
#include "e11_ui.h"

static const vtable_Object s_object_vtable = {
	E11UI_destructor,
	E11UI_clone,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	E11UI_defaultProc,
	E11UI_paint
};

void _handle_keyboard_keydown_event(PWidget self, PEvent system_event);


PE11UI E11UI_constructor(PE11UI self) {
	Event widget_event;
	DWORD i;

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* TODO: put defines later */
	self->widget.x =  0;
	self->widget.y = 16;

	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;

	/* Construct picture widgets */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		Picture_constructor(&self->icons[i]);
	}

	/* set every Function function pointers to NULL */
	memset(self->onFunction, '\0', sizeof(void (*)(PE11UI)) * NUMBER_OF_FUNCTIONS);

	/* No focused widget for the moment */
	self->focused_widget = NULL;
	
	/* Request the widget to be painted */
	widget_event.type = EVENT_PAINT;
	widget_event.real_event.widget_event.id = 0; /* Paint everything? */
	singleton_system()->vtable->enqueueEvent(singleton_system(), &widget_event);

	return self;
}

void E11UI_destructor(PObject self) {
	DWORD i;
	PE11UI real_self = (PE11UI)self;
	/* Call parent destructor */
	Widget_destructor(self);

	/* Destruct picture widgets */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		Picture_destructor(&real_self->icons[i].widget.object);
	}
}

PObject E11UI_clone(PObject self, PObject dst) {
	/* TODO. */

	/* Call parent clone */
	Widget_clone(self, dst);
	return dst;
}

DWORD E11UI_defaultProc(PWidget self, const PEvent system_event) {
	/* TODO. */
	PListNode it = NULL;
	PE11UI real_self = (PE11UI)self;
	PWidget current_child;
	Event custom_event;
	BYTE* keyboard_state = NULL;

	/* if there are no hot widget, pick up the first focusable widget */
	if(real_self->focused_widget == NULL) {
		real_self->focused_widget = self->childs.head;
		while(real_self->focused_widget != NULL) {
			current_child = ((PWidgetPtr)real_self->focused_widget->data)->widget;
			if(current_child->is_focusable) {
				/* if the current widget is focusable, inform it's been focused, & break the loop */
				custom_event.real_event.widget_event.id = current_child->id;
				custom_event.type = EVENT_FOCUS;
				singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
				break;
			}
			real_self->focused_widget = real_self->focused_widget->next;
		}
	}


	switch(system_event->type) {
	case EVENT_KEYBOARD_KDOWN:

		_handle_keyboard_keydown_event(self, system_event);	

		break;

	case EVENT_PAINT:
		if(system_event->real_event.widget_event.id == 0) {
			return Widget_defaultProc(self, system_event);
		} else {
			/* If the id equals 0, paint every childs, otherwise find the good one & paint it */
			it = self->childs.head;

			/* No id? dispatch the event to every child */
			while(it != NULL) {
				PWidget chld = ((PWidgetPtr)it->data)->widget;
				/* If the event is to the current child, forward it. */
				if(system_event->real_event.widget_event.id == chld->id) {
					chld->vtable->defaultProc(chld, system_event);
				}
				it = it->next;
			}
		}
		
		break;


	case EVENT_TIMER:
		current_child = ((PWidgetPtr)real_self->focused_widget->data)->widget;
		current_child->vtable->defaultProc(current_child, system_event);
		break;
	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void E11UI_paint(PWidget self, WORD base_x, WORD base_y) {
	PE11UI real_self = (PE11UI)self;
	WORD i;
	/* Call parent paint */
	Widget_paint(self, base_x, base_y);

	/* Draw icons */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		/* Paint each icon */
		real_self->icons[i].widget.vtable->paint(
			&real_self->icons[i].widget,
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)),
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6))
		);

		/* ... & the surronding rect */

		/* Top border */
		Lcd_drawRectangle(
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)) - BORDER_THICKNESS,
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*((WORD)i/6)) - BORDER_THICKNESS,
			real_self->icons[i].icon->header.width + BORDER_THICKNESS,
			BORDER_THICKNESS,
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW),
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW)
		);

		/* Right border */
		Lcd_drawRectangle(
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)) + real_self->icons[i].icon->header.width,
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6)) - BORDER_THICKNESS,
			BORDER_THICKNESS,
			real_self->icons[i].icon->header.height + BORDER_THICKNESS,
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW),
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW)
		);

		/* Bottom border */
		Lcd_drawRectangle(
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)),
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6)) + real_self->icons[i].icon->header.height,
			real_self->icons[i].icon->header.width + BORDER_THICKNESS,
			BORDER_THICKNESS,
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW),
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW)
		);

		/* Left border */
		Lcd_drawRectangle(
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)) - BORDER_THICKNESS,
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6)),
			BORDER_THICKNESS,
			real_self->icons[i].icon->header.height + BORDER_THICKNESS,
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW),
			(i < 6 ? BACKGROUND_FIRST_ROW : BACKGROUND_SECOND_ROW)
		);


	}
}

PPicture E11UI_getPicture(PE11UI self, DWORD index) {
	if(index >= 0 && index < NUMBER_OF_ICONS) {
		return &self->icons[index];
	} else {
		return NULL;
	}
}

void E11UI_setIcon(PE11UI self, DWORD index, const PPicture icon) {
	if(index >= 0 && index < NUMBER_OF_ICONS) {
		self->icons[index] = *icon;
	}
}

void _handle_keyboard_keydown_event(PWidget self, PEvent system_event) {
	PE11UI real_self = (PE11UI)self;
	PWidget current_child = NULL;
	BYTE* keyboard_state = NULL;
	Event custom_event;

	/* According to the key pressed */
	if(system_event->real_event.keyboard_event.code >= KEY_F1 &&
		system_event->real_event.keyboard_event.code <= KEY_F12) {
		/* Call the appropriate function */
		if(real_self->onFunction[system_event->real_event.keyboard_event.code - KEY_F1] != NULL) {
			real_self->onFunction[system_event->real_event.keyboard_event.code - KEY_F1](real_self);
		}
	} else {
		/* Need a hot widget to handle the event */
		if(real_self->focused_widget != NULL) {
			
			current_child = ((PWidgetPtr)(real_self->focused_widget)->data)->widget;

			if(system_event->real_event.keyboard_event.code == KEY_TAB) {
				/* Tab pressed: switch to the next hot widget */

				/* Inform the current widget that he's lost the focus */
				
				custom_event.real_event.widget_event.id = current_child->id;
				custom_event.type = EVENT_BLUR;
				singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
				keyboard_state = singleton_system()->vtable->getKeyState(singleton_system());

				while(1) {
					/* If alt is pressed, loop through widgets backward, otherwise go forward */
					if(keyboard_state[KEY_RSHIFT] || keyboard_state[KEY_LSHIFT]) {
						if(real_self->focused_widget->prev == NULL) {
							real_self->focused_widget = self->childs.tail;
						} else {
							real_self->focused_widget = real_self->focused_widget->prev;
						}
					} else {
						if(real_self->focused_widget->next == NULL) {
							real_self->focused_widget = self->childs.head;
						} else {
							real_self->focused_widget = real_self->focused_widget->next;
						}
					}

					current_child = ((PWidgetPtr)real_self->focused_widget->data)->widget;

					if(current_child->is_focusable) {
						/* if the current widget is focusable, break the loop */
						break;
					}
				}
			}

			/* Inform the new widget that it as gained the focus */
			custom_event.real_event.widget_event.id = current_child->id;
			custom_event.type = EVENT_FOCUS;
			singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);

			/* Forward the event to the focused widget */
			current_child->vtable->defaultProc(current_child, system_event);
		}
	}
}