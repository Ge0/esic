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
	

	/* FACTORY DESTROY */
	PaintersFactory_destroy();
	MarkingFontTTFactory_destroy();
	RasterIconFactory_destroy();
	RasterFontFactory_destroy();

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
