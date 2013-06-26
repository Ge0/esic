#include <esic/eresources/raster_icon_factory.h>
#include <ui/dotpen_ui.h>
#include <factories/xml_ui_factory.h>
#include <esic/eapi/system.h>
//#include <esic/egraphics/lcd_painter.h>

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) DotpenUI_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Widget) {
/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) DotpenUI_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/
	DotpenUI_defaultProc,
	DotpenUI_paint,
	Widget_handleWidgetEvent
};

static void (*s_onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI, void*) = {
#define E11_UI_FUNCTION(keycode) DotpenUI_on##keycode,
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
};

PDotpenUI DotpenUI_constructor(PDotpenUI self) {
	DWORD i;
	/* Calling parent constructor */
	E11UI_constructor(&self->e11ui);

	OBJECT(self)->size = sizeof(DotpenUI);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	WIDGET(self)->vtable = VTABLE_POINTER(Widget);


	/* Build the UI */
	XmlUiFactory_hydrateUI("dotpen_ui", &self->e11ui.widget);

	/* Set the 12 icons */
	self->e11ui.icons[11]->icon = RasterIconFactory_getRasterIcon("dotpen.ilcd");
	self->e11ui.icons[10]->icon = RasterIconFactory_getRasterIcon("edition.ilcd");
	self->e11ui.icons[9]->icon = RasterIconFactory_getRasterIcon("preview.ilcd");
	self->e11ui.icons[8]->icon = RasterIconFactory_getRasterIcon("list.ilcd");
	self->e11ui.icons[7]->icon = RasterIconFactory_getRasterIcon("settings.ilcd");
	self->e11ui.icons[6]->icon = RasterIconFactory_getRasterIcon("ask.ilcd");
	self->e11ui.icons[5]->icon = RasterIconFactory_getRasterIcon("dotpen.ilcd");
	self->e11ui.icons[4]->icon = RasterIconFactory_getRasterIcon("edition.ilcd");
	self->e11ui.icons[3]->icon = RasterIconFactory_getRasterIcon("preview.ilcd");
	self->e11ui.icons[2]->icon = RasterIconFactory_getRasterIcon("list.ilcd");
	self->e11ui.icons[1]->icon = RasterIconFactory_getRasterIcon("settings.ilcd");
	self->e11ui.icons[0]->icon = RasterIconFactory_getRasterIcon("ask.ilcd");

	for(i = 0; i < E11_NUMBER_OF_ICONS; ++i) {
		//assert(self->e11ui.icons[i].icon);

		/* Add icons to the list of childs widgets so they can be browsed with the TAB */
		Widget_addChild(&self->e11ui.widget, &self->e11ui.icons[i]->widget, FALSE);
	}

	
	//*(E11UI(self)->onFunction) = *s_onFunction;
	(E11UI(self)->onFunction) = s_onFunction;

	return self;
}

void DotpenUI_destructor(PObject self) {
	E11UI_destructor(self);
}

PObject DotpenUI_clone(PObject self, PObject dst) {
	/* TODO. */

	return dst;
}

BOOL DotpenUI_equalsTo(PObject self, PObject dst) {
	/* TODO. */

	return FALSE;
}

DWORD DotpenUI_hash(PObject self) {
	/* TODO. */
	return 0;
}

DWORD DotpenUI_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("DotpenUI");
	}

	return s_hash;
}

DWORD DotpenUI_defaultProc(PWidget self, const PEvent system_event) {
	switch(system_event->type) {
	
	default:
		return E11UI_defaultProc(self, system_event);
	}
}

void DotpenUI_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	E11UI_paint(self, painter, base_x, base_y);
}

void DotpenUI_onF1(PE11UI self, void* param) {

}

void DotpenUI_onF2(PE11UI self, void* param) {

}

void DotpenUI_onF3(PE11UI self, void* param) {

}

void DotpenUI_onF4(PE11UI self, void* param) {

}

void DotpenUI_onF5(PE11UI self, void* param) {

}

void DotpenUI_onF6(PE11UI self, void* param) {

}

void DotpenUI_onF7(PE11UI self, void* param) {

}

void DotpenUI_onF8(PE11UI self, void* param) {

}

void DotpenUI_onF9(PE11UI self, void* param) {

}

void DotpenUI_onF10(PE11UI self, void* param) {

}

void DotpenUI_onF11(PE11UI self, void* param) {

}

void DotpenUI_onF12(PE11UI self, void* param) {
	// Back to main UI
	Event user_event;
	Event_constructor(&user_event);

	user_event.type = EVENT_USER;
	user_event.real_event.user_event.type  = E11_EVENT_ACCEPT;
	user_event.real_event.user_event.param = WIDGET(self)->id;

	EsicPushEvent(&user_event);

	Event_destructor(OBJECT(&user_event));
}

