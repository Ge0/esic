#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
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
	DWORD i;

	/* Calling parent constructor */
	Widget_constructor(&self->widget);

	/* Filling vtables */
	self->widget.object.vtable = &s_object_vtable;
	self->widget.vtable        = &s_widget_vtable;

	/* Construct picture widgets */
	for(i = 0; i < NUMBER_OF_ICONS; i++) {
		Picture_constructor(&self->icons[i]);
	}

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
	switch(system_event->type) {
	
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
