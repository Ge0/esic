#include <esic/eapi/abstract_system.h>
#include <esic/eapi/raster_icon_factory.h>
#include <ui/main_ui.h>
#include <xml_ui_factory.h>

static const vtable_Object s_object_vtable = {
	MainUI_destructor,
	NULL,
	NULL,
	NULL
};

static const vtable_Widget s_widget_vtable = {
	MainUI_defaultProc,
	E11UI_paint				/* Keep the parent function */
};

/*
VTABLE_START(E11UI) {
#define E11_UI_FUNCTION(keycode) MainUI_on##keycode,
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
};
*/

static void (*s_onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI, void*) = {
#define E11_UI_FUNCTION(keycode) MainUI_on##keycode,
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
};

PMainUI MainUI_constructor(PMainUI self) {
	DWORD i;
	/* Calling parent constructor */
	E11UI_constructor(&self->e11ui);



	self->e11ui.widget.object.size = sizeof(MainUI);

	self->e11ui.widget.object.vtable = &s_object_vtable;
	self->e11ui.widget.vtable        = &s_widget_vtable;
	//E11UI(self)->vtable              = VTABLE_POINTER(E11UI);

	/* Build the UI */
	XmlUiFactory_hydrateUI("ui", &self->e11ui.widget);

	/* Set the 12 icons */
	self->e11ui.icons[0].icon = RasterIconFactory_getRasterIcon("dotpen.ilcd");
	self->e11ui.icons[1].icon = RasterIconFactory_getRasterIcon("edition.ilcd");
	self->e11ui.icons[2].icon = RasterIconFactory_getRasterIcon("preview.ilcd");
	self->e11ui.icons[3].icon = RasterIconFactory_getRasterIcon("list.ilcd");
	self->e11ui.icons[4].icon = RasterIconFactory_getRasterIcon("settings.ilcd");
	self->e11ui.icons[5].icon = RasterIconFactory_getRasterIcon("ask.ilcd");
	self->e11ui.icons[6].icon = RasterIconFactory_getRasterIcon("dotpen.ilcd");
	self->e11ui.icons[7].icon = RasterIconFactory_getRasterIcon("edition.ilcd");
	self->e11ui.icons[8].icon = RasterIconFactory_getRasterIcon("preview.ilcd");
	self->e11ui.icons[9].icon = RasterIconFactory_getRasterIcon("list.ilcd");
	self->e11ui.icons[10].icon = RasterIconFactory_getRasterIcon("settings.ilcd");
	self->e11ui.icons[11].icon = RasterIconFactory_getRasterIcon("ask.ilcd");

	for(i = 0; i < E11_NUMBER_OF_ICONS; ++i) {
		//assert(self->e11ui.icons[i].icon);

		/* Tmp: add icons to the list of childs widgets so they can be browsed with the TAB */
		Widget_addChild(&self->e11ui.widget, &self->e11ui.icons[i].widget);
	}

	
	*(E11UI(self)->onFunction) = *s_onFunction;

	return self;
}

void MainUI_destructor(PObject self) {
	E11UI_destructor(self);
}

DWORD MainUI_defaultProc(PWidget self, const PEvent system_event) {
	/* TODO. */
	switch(system_event->type) {
	
	default:
		return E11UI_defaultProc(self, system_event);
	}
	return 0;
}

void MainUI_onF1(PE11UI self, void* param) {
	SicPrintfDebug("F1 STROKE!\n");
}

void MainUI_onF2(PE11UI self, void* param) {

}

void MainUI_onF3(PE11UI self, void* param) {

}

void MainUI_onF4(PE11UI self, void* param) {

}

void MainUI_onF5(PE11UI self, void* param) {

}

void MainUI_onF6(PE11UI self, void* param) {

}

void MainUI_onF7(PE11UI self, void* param) {

}

void MainUI_onF8(PE11UI self, void* param) {

}

void MainUI_onF9(PE11UI self, void* param) {

}

void MainUI_onF10(PE11UI self, void* param) {

}

void MainUI_onF11(PE11UI self, void* param) {

}

void MainUI_onF12(PE11UI self, void* param) {

}

