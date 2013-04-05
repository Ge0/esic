#include <esic/eapi/system.h>
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
	WORD i;

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* TODO: put defines later */
	self->widget.x =  0;
	self->widget.y = 16;

	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;

	/* Construct picture widgets */
	for(i = 0; i < E11_NUMBER_OF_ICONS; i++) {
		Picture_constructor(&self->icons[i]);

		/* Id : 0x8000 + i + 1 */
		self->icons[i].widget.id = (WORD)(0x8000 + i + 1);

		/* Filling coordinates information */
		self->icons[i].widget.x = ICONS_BASE_X + (54*(i%ICONS_PER_LINE));
		self->icons[i].widget.y = ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6));
		self->icons[i].border_thickness = BORDER_THICKNESS;
		self->icons[i].border_color_hot = PICTURE_BORDER_COLOR_HOT;
		if(i < (E11_NUMBER_OF_ICONS / 2)) {
			self->icons[i].border_color = BACKGROUND_FIRST_ROW;
		} else {
			self->icons[i].border_color = BACKGROUND_SECOND_ROW;
		}
	}

	/* set every Function function pointers to NULL */
	memset(self->onFunction, '\0', sizeof(void (*)(PE11UI)) * E11_NUMBER_OF_ICONS);

	/* No focused widget for the moment */
	self->focused_widget = NULL;
	self->child_ui       = NULL;
	
	/* Request the widget to be painted */
	Event_constructor(&widget_event);
	widget_event.type = EVENT_PAINT;
	widget_event.real_event.widget_event.id = 0; /* Paint everything? */
	//singleton_system()->vtable->enqueueEvent(singleton_system(), &widget_event);
	EsicPushEvent(&widget_event);
	Event_destructor((PObject)&widget_event);

	return self;
}

void E11UI_destructor(PObject self) {
	DWORD i;
	PE11UI real_self = (PE11UI)self;
	/* Call parent destructor */
	Widget_destructor(self);

	/* Destruct picture widgets */
	for(i = 0; i < E11_NUMBER_OF_ICONS; i++) {
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

	Event_constructor(&custom_event);

	// Test: if there is a child user interface, immediately dispatch the events to it
	if(E11UI(self)->child_ui != NULL) {
		return WIDGET_VTABLE(E11UI(self)->child_ui)->defaultProc(WIDGET(E11UI(self)->child_ui), system_event);
	}

	/* if there are no hot widget, pick up the first focusable widget */
	if(real_self->focused_widget == NULL) {
		real_self->focused_widget = self->childs.head;
		while(real_self->focused_widget != NULL) {
			current_child = ((PWidgetPtr)real_self->focused_widget->data)->widget;
			if(current_child->is_focusable) {
				/* if the current widget is focusable, inform it's been focused, & break the loop */
				Event_constructor(&custom_event);
				custom_event.real_event.widget_event.id = current_child->id;
				//custom_event.type = EVENT_FOCUS;
				custom_event.type = EVENT_WIDGET;
				custom_event.real_event.widget_event.type = WE_FOCUS;
				//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
				EsicPushEvent(&custom_event);
				Event_destructor(&custom_event.object);
				break;
			}
			real_self->focused_widget = real_self->focused_widget->next;
		}
	}


	switch(system_event->type) {
	case EVENT_KEYBOARD_KDOWN:

		_handle_keyboard_keydown_event(self, system_event);	

		break;

	case EVENT_WIDGET:
	//case EVENT_PAINT:
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
					break;
				}
				it = it->next;
			}
		}
		
		break;

	case EVENT_TIMER:
		
		/*
		current_child = ((PWidgetPtr)real_self->focused_widget->data)->widget;
		current_child->vtable->defaultProc(current_child, system_event);
		*/

		/* Forward the event to every childs */
		it = self->childs.head;
		while(it != NULL) {
			PWidget chld = ((PWidgetPtr)it->data)->widget;

			chld->vtable->defaultProc(chld, system_event);

			it = it->next;
		}

		break;

	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void E11UI_paint(PWidget self, WORD base_x, WORD base_y) {
	PE11UI real_self = (PE11UI)self;
	//WORD i;
	/* Call parent paint */
	Widget_paint(self, base_x, base_y);

	/* Draw icons */
	//for(i = 0; i < E11_NUMBER_OF_ICONS; i++) {
		/* Paint each icon */
		/*
		real_self->icons[i].widget.vtable->paint(
			&real_self->icons[i].widget,
			ICONS_BASE_X + (54*(i%ICONS_PER_LINE)),
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6))
		);
		*/


	//	}
}

PPicture E11UI_getPicture(PE11UI self, DWORD index) {
	if(index >= 0 && index < E11_NUMBER_OF_ICONS) {
		return &self->icons[index];
	} else {
		return NULL;
	}
}

void E11UI_setIcon(PE11UI self, DWORD index, const PPicture icon) {
	if(index >= 0 && index < E11_NUMBER_OF_ICONS) {
		self->icons[index] = *icon;
	}
}

void _handle_keyboard_keydown_event(PWidget self, PEvent system_event) {
	PE11UI real_self = (PE11UI)self;
	PWidget current_child = NULL;
	const BYTE* keyboard_state = NULL;
	Event custom_event;

	/* According to the key pressed */
	if(system_event->real_event.keyboard_event.code >= KEY_F1 &&
		system_event->real_event.keyboard_event.code <= KEY_F12) {
		/* Call the appropriate function */
		if(real_self->onFunction[system_event->real_event.keyboard_event.code - KEY_F1] != NULL) {
			real_self->onFunction[system_event->real_event.keyboard_event.code - KEY_F1](real_self, NULL);
		}
	} else {
		/* Need a hot widget to handle the event */
		if(real_self->focused_widget != NULL) {
			
			current_child = ((PWidgetPtr)(real_self->focused_widget)->data)->widget;

			if(system_event->real_event.keyboard_event.code == KEY_TAB) {
				/* Tab pressed: switch to the next hot widget */

				/* Inform the current widget that he's lost the focus */
				Event_constructor(&custom_event);
				custom_event.real_event.widget_event.id = current_child->id;
				custom_event.real_event.widget_event.type = WE_BLUR;
				//custom_event.type = EVENT_BLUR;
				custom_event.type = EVENT_WIDGET;
				//singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);
				EsicPushEvent(&custom_event);
				//keyboard_state = singleton_system()->vtable->getKeyState(singleton_system());
				keyboard_state = EsicGetKeyboardState();
				Event_destructor(&custom_event.object);

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
				/* Inform the new widget that it as gained the focus */
				Event_constructor(&custom_event);
				custom_event.real_event.widget_event.id = current_child->id;
				//custom_event.type = EVENT_FOCUS;
				custom_event.real_event.widget_event.type = WE_FOCUS;
				custom_event.type = EVENT_WIDGET;
				EsicPushEvent(&custom_event);
				Event_destructor(&custom_event.object);

				
			}

			/* In every cases, forward the event to the focused widget */
			current_child->vtable->defaultProc(current_child, system_event);
		}
	}
}