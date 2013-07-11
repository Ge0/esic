

#include <ui/main_ui.h>
#include <ui/dotpen_ui.h>
#include <ui/preview_ui.h>

#include <factories/xml_ui_factory.h>

#include <esic/eapi/system.h>
#include <esic/eapi/misc.h>
#include <esic/egraphics/pixel.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/graphicsview/canvas.h>
#include <esic/eresources/raster_icon_factory.h>
#include <libsic/marking/text_marking_line.h>
#include <factories/marking_font_tt_factory.h>

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

static void (*s_onFunction[E11_NUMBER_OF_FUNCTIONS])(PE11UI) = {
#define E11_UI_FUNCTION(keycode) MainUI_on##keycode,
	E11_UI_FUNCTION_LIST
#undef E11_UI_FUNCTION
};

/* Test */
PTextMarkingLine GetTestMarkingLine(void);
void _feed_canvas_markingline(PMainUI self, PTextMarkingLine marking_line);
void _feed_canvas_markingchar(PMainUI self, PMarkingFontTT font, char ch, long x, long y, long width, long height);


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

	/* Set the icons */
	Picture_setIcon(self->e11ui.icons[6], RasterIconFactory_getRasterIcon("edition_black.ilcd"));
	Picture_setIcon(self->e11ui.icons[7], RasterIconFactory_getRasterIcon("variables_black.ilcd"));
	Picture_setIcon(self->e11ui.icons[8], RasterIconFactory_getRasterIcon("preview_black.ilcd"));
	Picture_setIcon(self->e11ui.icons[9], RasterIconFactory_getRasterIcon("list_black.ilcd"));
	Picture_setIcon(self->e11ui.icons[10], RasterIconFactory_getRasterIcon("settings_black.ilcd"));
	Picture_setIcon(self->e11ui.icons[11], RasterIconFactory_getRasterIcon("ask_black.ilcd"));

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

	// Test rendering a marking line
	_feed_canvas_markingline(E11_MAINUI(self), GetTestMarkingLine());

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

void MainUI_onF1(PE11UI self) {
	
}

void MainUI_onF2(PE11UI self) {

}

void MainUI_onF3(PE11UI self) {

}

void MainUI_onF4(PE11UI self) {

}

void MainUI_onF5(PE11UI self) {

}

void MainUI_onF6(PE11UI self) {

}

void MainUI_onF7(PE11UI self) {
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

void MainUI_onF8(PE11UI self) {

	/* TEST */
	PPixel shape;
	NEW(shape, Pixel);
	shape->color = 0; /* BLACK */
	SHAPE(shape)->position.x = RandLong(1, 10);
	SHAPE(shape)->position.y = RandLong(1, 10);
	GraphicsScene_addShape(&E11_MAINUI(self)->marking_file_scene, SHAPE(shape), TRUE);

	// notify
	Observable_notify(OBSERVABLE(&E11_MAINUI(self)->marking_file_scene), NULL);
}

void MainUI_onF9(PE11UI self) {
	Event user_event;
	PPreviewUI preview_ui = NULL;


	NEW(preview_ui, PreviewUI);
	
	// Send an event to the upper layer to inform the creation of the child
	Event_constructor(&user_event);
	user_event.type = EVENT_USER;
	user_event.real_event.user_event.type  = E11_EVENT_CREATE_CHILD;
	user_event.real_event.user_event.param = (DWORD)preview_ui;

	EsicPushEvent(&user_event);

	Event_destructor(OBJECT(&user_event));
}

void MainUI_onF10(PE11UI self) {

}

void MainUI_onF11(PE11UI self) {

}

void MainUI_onF12(PE11UI self) {

}


// --------------------------------------------------------------------

PTextMarkingLine GetTestMarkingLine(void) {
	static TextMarkingLine marking_line;
	static int inited = 0;

	if(inited == 0) {
	
		TextMarkingLine_constructor(&marking_line);
	
		MARKINGLINE(&marking_line)->x      = 0;
		MARKINGLINE(&marking_line)->y      = 0;
		MARKINGLINE(&marking_line)->width  = 80;
		MARKINGLINE(&marking_line)->height = 80;

		ZString_setData(&marking_line.content, "SIC MARKING");
		marking_line.marking_font = MarkingFontTTFactory_getMarkingFontTT("OCR");

		inited = 1;
	}
	
	return &marking_line;
}

void _feed_canvas_markingline(PMainUI self, PTextMarkingLine marking_line) {

	const int resolution = 4.0;
	int offset = 1;
	DWORD i = 0;
	for(i = 0; i < marking_line->content.size; ++i) {
		char ch = marking_line->content.data[i];
		if(ch >= ' ') {
			_feed_canvas_markingchar(
				self,
				marking_line->marking_font,
				ch,
				offset + (MARKINGLINE(marking_line)->x/10)*resolution,
				(MARKINGLINE(marking_line)->y/10)*resolution,
				MARKINGLINE(marking_line)->width,
				MARKINGLINE(marking_line)->height
			);
			offset += (MARKINGLINE(marking_line)->width/resolution);
		}
	}

	// Notify the observer
	Observable_notify(OBSERVABLE(&E11_MAINUI(self)->marking_file_scene), NULL);
}

void _feed_canvas_markingchar(PMainUI self, PMarkingFontTT font, char ch, long x, long y, long width, long height) {
	DWORD i;
	ch -= ' ';

	for(i = 0; i < font->characters[ch].number_of_points; ++i) {
		if(font->characters[ch].coords[i].x1 >= 0) {
			/*
			LcdSetPixel(
				x + (DWORD)((font->characters[ch].coords[i].x1/120.0) * (width/4.0)),
				y + (DWORD)(((180-font->characters[ch].coords[i].y1)/120.0) * (height/4.0)),
				0
			);
			*/

			/* TEST */
			PPixel shape;
			NEW(shape, Pixel);
			shape->color = 0; /* BLACK */
			SHAPE(shape)->position.x = x + (DWORD)((font->characters[ch].coords[i].x1/120.0) * (width/4.0));
			SHAPE(shape)->position.y = y + (DWORD)(((180-font->characters[ch].coords[i].y1)/120.0) * (height/4.0));
			GraphicsScene_addShape(&E11_MAINUI(self)->marking_file_scene, SHAPE(shape), TRUE);

		}
	}

}
