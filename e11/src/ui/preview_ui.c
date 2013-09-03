#include <ui/main_ui.h>
#include <ui/preview_ui.h>
#include <esic/eapi/system.h>
#include <esic/egraphics/pixel.h>
#include <esic/egui/graphicsview/canvas.h>
#include <esic/egui/graphicsview/graphics_scene.h>
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

// Private functions
static void _feed_canvas_markingline(PPreviewUI self, PTextMarkingLine marking_line);
static void _feed_canvas_markingchar(PPreviewUI self, PMarkingFontTT font, char ch, long x, long y, long width, long height);
static void _build_test_marking_line(PTextMarkingLine marking_line);

static void _repaint_canvas(PPreviewUI self);

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

	// Test
	_build_test_marking_line(&self->test_marking_line);
	

	/* Test: create a graphics scene that will contain shapes */
	GraphicsScene_constructor(&self->marking_file_scene);

	/* Assign it to our canvas */
	self->canvas = (PCanvas)Widget_findChildById(WIDGET(self), PREVIEW_UI_ID_CANVAS);
	if(self->canvas != NULL) {
		Canvas_setGraphicsScene(self->canvas, &self->marking_file_scene);
	}

	SicAssert(self->canvas != NULL);

	// Test rendering a marking line
	_feed_canvas_markingline(E11_PREVIEWUI(self), &self->test_marking_line);

	return self;

}

void PreviewUI_destructor(PObject self) {
	// Test
	GraphicsScene_destructor(OBJECT(&E11_PREVIEWUI(self)->marking_file_scene));
	TextMarkingLine_destructor(OBJECT(&E11_PREVIEWUI(self)->test_marking_line));
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
	
	// Move to the top
	if(E11_PREVIEWUI(self)->canvas->y_offset >= 10) {
		E11_PREVIEWUI(self)->canvas->y_offset -= 10;
		_repaint_canvas(E11_PREVIEWUI(self));
	}
	
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
	// Move to the left
	if(E11_PREVIEWUI(self)->canvas->x_offset >= 10) {
		E11_PREVIEWUI(self)->canvas->x_offset -= 10;
		_repaint_canvas(E11_PREVIEWUI(self));
	}
}

void PreviewUI_onF10(PE11UI self) {
	// Move to the bottom
	E11_PREVIEWUI(self)->canvas->y_offset += 10;
	_repaint_canvas(E11_PREVIEWUI(self));
}

void PreviewUI_onF11(PE11UI self) {
	// Move to the right
	E11_PREVIEWUI(self)->canvas->x_offset += 10;
	_repaint_canvas(E11_PREVIEWUI(self));
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

static void _repaint_canvas(PPreviewUI self) {
	Event paint_event;
	Event_constructor(&paint_event);

	paint_event.type = EVENT_WIDGET;
	paint_event.real_event.widget_event.type = WE_PAINT;
	paint_event.real_event.widget_event.id = WIDGET(self->canvas)->id;

	EsicPushEvent(&paint_event);

	Event_destructor(OBJECT(&paint_event));
}


static void _feed_canvas_markingline(PPreviewUI self, PTextMarkingLine marking_line) {

	const int resolution = 4;
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
	Observable_notify(OBSERVABLE(&E11_PREVIEWUI(self)->marking_file_scene), NULL);
}

void _feed_canvas_markingchar(PPreviewUI self, PMarkingFontTT font, char ch, long x, long y, long width, long height) {
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
			GraphicsScene_addShape(&E11_PREVIEWUI(self)->marking_file_scene, SHAPE(shape), TRUE);

		}
	}

}

static void _build_test_marking_line(PTextMarkingLine marking_line) {
	TextMarkingLine_constructor(marking_line);

	MARKINGLINE(marking_line)->x      = 0;
	MARKINGLINE(marking_line)->y      = 0;
	MARKINGLINE(marking_line)->width  = 80;
	MARKINGLINE(marking_line)->height = 80;

	ZString_setData(&marking_line->content, "SIC MARKING");
	marking_line->marking_font = MarkingFontTTFactory_getMarkingFontTT("OCR");

}