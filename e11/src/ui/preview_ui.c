#include <ui/main_ui.h>
#include <ui/preview_ui.h>
#include <esic/eapi/system.h>
#include <esic/eresources/raster_icon_factory.h>
#include <libsic/marking/text_marking_line.h>
#include <factories/marking_font_tt_factory.h>
#include <factories/xml_ui_factory.h>

VTABLE_START(Object) {
	PreviewUI_destructor,
	NULL,
	NULL,
	NULL,
	NULL
};

VTABLE_START(Widget) {
	PreviewUI_defaultProc,
	E11UI_paint,
	NULL
};

static void (*s_onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI) = {
#define E11_UI_FUNCTION(keycode) PreviewUI_on##keycode,
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
};

PPreviewUI PreviewUI_constructor(PPreviewUI self) {

	DWORD i;

	/* Call parent constructor */
	E11UI_constructor(E11UI(self));

	OBJECT(self)->size = sizeof(PreviewUI);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	WIDGET(self)->vtable = VTABLE_POINTER(Widget);


	/* Build the UI */
	XmlUiFactory_hydrateUI("preview_ui", &self->e11ui.widget);

	/* Set the icons */
	Picture_setIcon(self->e11ui.icons[1], RasterIconFactory_getRasterIcon("zoom_in.ilcd"));
	Picture_setIcon(self->e11ui.icons[3], RasterIconFactory_getRasterIcon("arrow_up.ilcd"));
	Picture_setIcon(self->e11ui.icons[5], RasterIconFactory_getRasterIcon("cancel.ilcd"));
	Picture_setIcon(self->e11ui.icons[7], RasterIconFactory_getRasterIcon("zoom_out.ilcd"));
	Picture_setIcon(self->e11ui.icons[8], RasterIconFactory_getRasterIcon("arrow_left.ilcd"));
	Picture_setIcon(self->e11ui.icons[9], RasterIconFactory_getRasterIcon("arrow_down.ilcd"));
	Picture_setIcon(self->e11ui.icons[10], RasterIconFactory_getRasterIcon("arrow_right.ilcd"));
	Picture_setIcon(self->e11ui.icons[11], RasterIconFactory_getRasterIcon("accept.ilcd"));

	for(i = 0; i < E11_NUMBER_OF_ICONS; ++i) {
		//assert(self->e11ui.icons[i].icon);

		/* Add icons to the list of childs widgets so they can be browsed with the TAB */
		Widget_addChild(&self->e11ui.widget, &self->e11ui.icons[i]->widget, FALSE);
	}

	
	//*(E11UI(self)->onFunction) = *s_onFunction;
	(E11UI(self)->onFunction) = s_onFunction;

	return self;

}

void PreviewUI_destructor(PObject self) {
	E11UI_destructor(self);
}

DWORD PreviewUI_defaultProc(PWidget self, const PEvent system_event) {
	switch(system_event->type) {
	
	default:
		return E11UI_defaultProc(self, system_event);
	}
}

void PreviewUI_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	E11UI_paint(self, painter, base_x, base_y);
}

void PreviewUI_onF1(PE11UI self) {

}

void PreviewUI_onF2(PE11UI self) {

}

void PreviewUI_onF3(PE11UI self) {

}

void PreviewUI_onF4(PE11UI self) {

}

void PreviewUI_onF5(PE11UI self) {

}

void PreviewUI_onF6(PE11UI self) {

}

void PreviewUI_onF7(PE11UI self) {

}

void PreviewUI_onF8(PE11UI self) {

}

void PreviewUI_onF9(PE11UI self) {

}

void PreviewUI_onF10(PE11UI self) {

}

void PreviewUI_onF11(PE11UI self) {

}

void PreviewUI_onF12(PE11UI self) {
	// Back to main UI
	Event user_event;
	Event_constructor(&user_event);

	user_event.type = EVENT_USER;
	user_event.real_event.user_event.type  = E11_EVENT_ACCEPT;
	user_event.real_event.user_event.param = WIDGET(self)->id;

	EsicPushEvent(&user_event);

	Event_destructor(OBJECT(&user_event));
}
