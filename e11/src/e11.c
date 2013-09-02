#include "e11.h"
#include <stdlib.h>
#include <time.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>
#include <esic/eapi/system.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/eresources/raster_font_factory.h>
#include <esic/eresources/raster_icon_factory.h>
#include <factories/marking_font_tt_factory.h>
#include <esic/egraphics/factories/painters_factory.h>
#include <esic/egraphics/geometrical_renderer.h>
//#include <esic/egui/default_widget_renderer.h>
#include <esic/etools/zstring.h>
#include <fatfs/ff.h>
//#include <esic/egraphics/lcd_painter.h>
/* TEST END */

/* OTHER TEST START */
#include <ui/main_ui.h>
#include <factories/xml_ui_factory.h>
#include <libsic/marking/text_marking_line.h>

void TestDrawString(PMarkingFontTT font, DWORD base_x, DWORD base_y, const char* string);
void TestDrawChar(PMarkingFontTT font, DWORD base_x, DWORD base_y, char ch);

PTextMarkingLine GetTestMarkingLine(void);
void TestDrawMarkingLine(PTextMarkingLine marking_line);
void TestDrawMarkingChar(PMarkingFontTT font, char ch, long x, long y, long width, long height);

void _init_painters();

void e11() {

	PTextMarkingLine marking_line = NULL;

	/* FACTORY INIT */
	srand((unsigned int)time(NULL));
	RasterFontFactory_init();
	RasterIconFactory_init();
	MarkingFontTTFactory_init();
	PaintersFactory_init();

	_init_painters();


	/* Splashscreen */
	_e11_splashscreen();

	/* And main loop */
	_e11_mainloop();
	
	/* Free the resources */
	/*DELETE(widget_renderer);*/

	/* Test */
	//FreeLcdPainter();

	marking_line = GetTestMarkingLine();

	/* FACTORY DESTROY */
	PaintersFactory_destroy();
	MarkingFontTTFactory_destroy();
	RasterIconFactory_destroy();
	RasterFontFactory_destroy();

	/* Test */
	
	//DELETE(marking_line);
	TextMarkingLine_destructor(OBJECT(marking_line));

	SicHeapDump();
}



void _e11_splashscreen() {

}

void _e11_mainloop() {
	BOOL looping = TRUE;
	DWORD ticks1, ticks2;
	PMarkingFontTT ocr_font;	/* TEST */
	PListNode current_painter;

	MainUI main_ui;
	
	MainUI_constructor(&main_ui);
	
	/* TEST */
	ocr_font = MarkingFontTTFactory_getMarkingFontTT("OCR");

	current_painter = GetPainters()->head;
	while(current_painter != NULL) {
		main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, PAINTER(current_painter->data), 0, 0);
		current_painter = current_painter->next;
	}

	ticks1 = EsicGetTicks();
	while(looping) {
		Event system_event;

		Event_constructor(&system_event);

		//TestDrawMarkingLine(GetTestMarkingLine());

		if(EsicPollEvent(&system_event)) {
			
			switch(system_event.type) {
			case EVENT_QUIT:
 				looping = (BOOL)!looping;
				break;

			default:
				WIDGET_VTABLE(&main_ui)->defaultProc(WIDGET(&main_ui), &system_event);
			}
		}

		/* test */
		ticks2 = EsicGetTicks();
		if(ticks2 - ticks1 >= 550) {
			/* One second elapsed */
			Event timer_event;
			Event_constructor(&timer_event);
			timer_event.type = EVENT_TIMER;
			timer_event.real_event.timer_event.id = 1;
			EsicPushEvent(&timer_event);
			ticks1 = ticks2;
			Event_destructor(&timer_event.object);
		}

		LcdUpdate();

		Event_destructor(OBJECT(&system_event));

	}
	
	MainUI_destructor(&main_ui.e11ui.widget.object);

}

/*
void TestDrawChar(PMarkingFontTT font, DWORD base_x, DWORD base_y, char ch) {
	DWORD i;

	ch -= ' ';

	for(i = 0; i < font->characters[ch].number_of_points; ++i) {
		if(font->characters[ch].coords[i].x1 >= 0) {
			LcdSetPixel(
				base_x + (DWORD)(font->characters[ch].coords[i].x1/7.0f),
				base_y + (DWORD)(((180) - font->characters[ch].coords[i].y1)/7.0f),
				0
			);
		}

		
		
		if(font->characters[ch].coords[i].x2 >= 0) {
			LcdSetPixel(
				base_x + (DWORD)(font->characters[ch].coords[i].x2/7.0),
				base_y + (DWORD)(((180) - font->characters[ch].coords[i].y2)/7.0),
				0
			);
		}
		
		

	}

}

void TestDrawString(PMarkingFontTT font, DWORD base_x, DWORD base_y, const char* string) {
	DWORD i;
	DWORD len = strlen(string);

	for(i = 0; i < len; ++i) {
		if(string[i] > ' ') {
			TestDrawChar(font, base_x, base_y, string[i]);
			base_x += (DWORD)(font->characters[string[i] - ' '].width/7.0);
		} else {
			base_x += 10;
		}
	}
}
*/


void TestDrawMarkingLine(PTextMarkingLine marking_line) {

	int base_x = 30;
	int base_y = 50;
	const int resolution = 4.0;
	int offset = 0;
	DWORD i = 0;
	for(i = 0; i < marking_line->content.size; ++i) {
		char ch = marking_line->content.data[i];
		if(ch >= ' ') {
			TestDrawMarkingChar(
				marking_line->marking_font,
				ch,
				base_x + /* offset + */ (MARKINGLINE(marking_line)->x/10)*resolution,
				base_y +  (MARKINGLINE(marking_line)->y/10)*resolution,
				MARKINGLINE(marking_line)->width,
				MARKINGLINE(marking_line)->height
			);
			base_x += (MARKINGLINE(marking_line)->width/resolution);
			//offset += (MARKINGLINE(marking_line)->width/resolution);
		}
	}

}

void TestDrawMarkingChar(PMarkingFontTT font, char ch, long x, long y, long width, long height) {
	DWORD i;
	ch -= ' ';

	for(i = 0; i < font->characters[ch].number_of_points; ++i) {
		if(font->characters[ch].coords[i].x1 >= 0) {
			LcdSetPixel(
				x + (DWORD)((font->characters[ch].coords[i].x1/120.0) * (width/4.0)),
				y + (DWORD)(((180-font->characters[ch].coords[i].y1)/120.0) * (height/4.0)),
				0
			);
		}
	}

}

void _init_painters() {
	Painter painter;
	PGeometricalRenderer renderer;

	/* inits a geometrical painter to the lcd screen */
	Painter_constructor(&painter);
	painter.clip.height = painter.clip.width = 0xFFFFFFFF;
	painter.clip.x      = painter.clip.y = 0;

	/* Default color is black */
	painter.color = RGB_16B(0,0,0);
	
	/* Default font is 6x8.flcd */
	painter.font  = RasterFontFactory_getRasterFont("6x8.flcd");

	NEW(renderer, GeometricalRenderer);

	/* Set the geometrical renderer */
	painter.renderer = RENDERER(renderer);

	/* Register the painter */
	PaintersFactory_registerPainter(&painter);

	Painter_destructor(OBJECT(&painter));
}

/*
PTextMarkingLine GetTestMarkingLine(void) {
	static TextMarkingLine marking_line;
	static int inited = 0;

	if(inited == 0) {
	
		TextMarkingLine_constructor(&marking_line);
	
		MARKINGLINE(&marking_line)->x      = 0;
		MARKINGLINE(&marking_line)->y      = 0;
		MARKINGLINE(&marking_line)->width  = 60;
		MARKINGLINE(&marking_line)->height = 60;

		ZString_setData(&marking_line.content, "SIC MARKING");
		marking_line.marking_font = MarkingFontTTFactory_getMarkingFontTT("OCR");

		inited = 1;
	}
	
	return &marking_line;
}
*/