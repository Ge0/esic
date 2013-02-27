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


PE11UI E11UI_constructor(PE11UI self) {
	Event widget_event;
	DWORD i;

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* Define later */
	self->widget.x =  0;
	self->widget.y = 16;

	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;

	/* Construct picture widgets */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		Picture_constructor(&self->icons[i]);
	}

	//self->hot_widget = NULL;

	
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
	static PListNode hot_widget = NULL;
	PListNode it = NULL;
	PE11UI real_self = (PE11UI)self;
	PWidget current_child;
	Event custom_event;
	/* if there are no hot widget, pick up the first focusable widget */
	if(hot_widget == NULL) {
		hot_widget = self->childs.head;
		while(hot_widget != NULL) {
			current_child = ((PWidgetPtr)hot_widget->data)->widget;
			if(current_child->is_focusable) {
				/* if the current widget is focusable, break the loop */
				break;
			}
			hot_widget = hot_widget->next;
		}
	}


	switch(system_event->type) {
	case EVENT_KEYBOARD_KDOWN:
		/* Need a hot widget to handle the event */
		if(hot_widget != NULL) {
			
			current_child = ((PWidgetPtr)hot_widget->data)->widget;

			if(system_event->real_event.keyboard_event.code == KEY_TAB) {
				/* Tab pressed: switch to the next hot widget */

				/* Inform the current widget that he's lost the focus */
				
				custom_event.real_event.widget_event.id = current_child->id;
				custom_event.type = EVENT_BLUR;
				singleton_system()->vtable->enqueueEvent(singleton_system(), &custom_event);

				while(1) {
					if(hot_widget->next == NULL) {
						hot_widget = self->childs.head;
					} else {
						hot_widget = hot_widget->next;
					}

					current_child = ((PWidgetPtr)hot_widget->data)->widget;

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

			current_child->vtable->defaultProc(current_child, system_event);
		}

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
		current_child = ((PWidgetPtr)hot_widget->data)->widget;
		current_child->vtable->defaultProc(current_child, system_event);
		break;
	default:
		return Widget_defaultProc(self, system_event);
	}
	return 0;
}

void E11UI_paint(PWidget self, WORD base_x, WORD base_y) {
	PE11UI real_self = (PE11UI)self;
	DWORD i;
	/* Call parent paint */
	Widget_paint(self, base_x, base_y);

	/* Draw icons */
	for(i = 0; i < 12; i++) {
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
			ICONS_BASE_Y + (MARGIN_SECOND_ICON_LINE*(i/6)) - BORDER_THICKNESS,
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
