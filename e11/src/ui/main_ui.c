

#include <ui/main_ui.h>
#include <ui/dotpen_ui.h>

#include <factories/xml_ui_factory.h>

#include <esic/eapi/system.h>
#include <esic/eapi/misc.h>
#include <esic/egraphics/pixel.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/graphicsview/canvas.h>
#include <esic/eresources/raster_icon_factory.h>

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
	PWidget canvas;
	/* Calling parent constructor */
	E11UI_constructor(&self->e11ui);



	self->e11ui.widget.object.size = sizeof(MainUI);

	self->e11ui.widget.object.vtable = &s_object_vtable;
	self->e11ui.widget.vtable        = &s_widget_vtable;
	//E11UI(self)->vtable              = VTABLE_POINTER(E11UI);

	/* Build the UI */
	XmlUiFactory_hydrateUI("main_ui", &self->e11ui.widget);

	/* Set the 12 icons */
	self->e11ui.icons[6]->icon = RasterIconFactory_getRasterIcon("edition_black.ilcd");
	self->e11ui.icons[7]->icon = RasterIconFactory_getRasterIcon("variables_black.ilcd");
	self->e11ui.icons[8]->icon = RasterIconFactory_getRasterIcon("preview_black.ilcd");
	self->e11ui.icons[9]->icon = RasterIconFactory_getRasterIcon("list_black.ilcd");
	self->e11ui.icons[10]->icon = RasterIconFactory_getRasterIcon("settings_black.ilcd");
	self->e11ui.icons[11]->icon = RasterIconFactory_getRasterIcon("ask_black.ilcd");

	/*
	self->e11ui.icons[6]->icon = RasterIconFactory_getRasterIcon("dotpen_black.ilcd");
	self->e11ui.icons[7]->icon = RasterIconFactory_getRasterIcon("edition_black.ilcd");
	self->e11ui.icons[8]->icon = RasterIconFactory_getRasterIcon("preview_black.ilcd");
	self->e11ui.icons[9]->icon = RasterIconFactory_getRasterIcon("list_black.ilcd");
	self->e11ui.icons[10]->icon = RasterIconFactory_getRasterIcon("settings_black.ilcd");
	self->e11ui.icons[11]->icon = RasterIconFactory_getRasterIcon("ask_black.ilcd");
	*/

	for(i = 0; i < E11_NUMBER_OF_ICONS; ++i) {
		//assert(self->e11ui.icons[i].icon);

		/* Add icons to the list of childs widgets so they can be browsed with the TAB */
		if(self->e11ui.icons[i]->icon != NULL) {
			Widget_addChild(&self->e11ui.widget, &self->e11ui.icons[i]->widget, FALSE);
		}
	}

	/* Assigning functions */
	(E11UI(self)->onFunction) = s_onFunction;

	/* Test: create a graphics scene that will contain shapes */
	GraphicsScene_constructor(&self->marking_file_scene);

	/* Assign it to our canvas */
	canvas = Widget_findChildById(WIDGET(self), MAIN_UI_ID_CANVAS);
	if(canvas != NULL) {
		Canvas_setGraphicsScene(CANVAS(canvas), &self->marking_file_scene);
	}

	return self;
}

void MainUI_destructor(PObject self) {
	E11UI_destructor(self);

	GraphicsScene_destructor(OBJECT(&E11_MAINUI(self)->marking_file_scene));

	/* Test */
	if(E11UI(self)->child_ui != NULL) {
		DELETE(E11UI(self)->child_ui);
		E11UI(self)->child_ui = NULL;
	}
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
	Event user_event;
	PDotpenUI dotpen_ui = NULL;


	NEW(dotpen_ui, DotpenUI);
	
	// Send an event to the upper layer to inform the creation of the child
	Event_constructor(&user_event);
	user_event.type = EVENT_USER;
	user_event.real_event.user_event.type  = E11_EVENT_CREATE_CHILD;
	user_event.real_event.user_event.param = (DWORD)dotpen_ui;

	EsicPushEvent(&user_event);

	Event_destructor(OBJECT(&user_event));
}

void MainUI_onF8(PE11UI self, void* param) {

	/* TEST */
	PPixel shape;
	NEW(shape, Pixel);
	shape->color = 0; /* BLACK */
	shape->coords.x = RandLong(1, 319);
	shape->coords.y = RandLong(1, 219);
	GraphicsScene_addShape(&E11_MAINUI(self)->marking_file_scene, SHAPE(shape), TRUE);
}

void MainUI_onF9(PE11UI self, void* param) {

}

void MainUI_onF10(PE11UI self, void* param) {

}

void MainUI_onF11(PE11UI self, void* param) {

}

void MainUI_onF12(PE11UI self, void* param) {

}

