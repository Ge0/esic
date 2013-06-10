#include "e11.h"
#include <stdlib.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>
#include <esic/eapi/system.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/eresources/raster_font_factory.h>
#include <esic/eresources/raster_icon_factory.h>
#include <factories/marking_font_tt_factory.h>
#include <esic/egui/default_widget_renderer.h>
#include <esic/etools/zstring.h>
#include <fatfs/ff.h>
/* TEST END */

/* OTHER TEST START */
#include <ui/main_ui.h>
#include <factories/xml_ui_factory.h>

void TestDrawString(PMarkingFontTT font, DWORD base_x, DWORD base_y, const char* string);
void TestDrawChar(PMarkingFontTT font, DWORD base_x, DWORD base_y, char ch);

void e11() {
	PDefaultWidgetRenderer widget_renderer = NULL;

	
	/* LCD INIT */
	/*
	LcdInit(480, 272, 16, LCD_DOUBLEBUF);
	LcdFill(RGB_16B(240,240,240));
	*/

	/* FACTORY INIT */
	RasterFontFactory_init();
	RasterIconFactory_init();
	MarkingFontTTFactory_init();


	/* WIDGET RENDERER INIT */
	NEW(widget_renderer, DefaultWidgetRenderer);
	SetDefaultWidgetRenderer((PAbstractWidgetRenderer)widget_renderer);

	/* Splashscreen */
	_e11_splashscreen();

	/* And main loop */
	_e11_mainloop();
	
	/* Free the resources */
	DELETE(widget_renderer);

	/* FACTORY DESTROY */
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

	MainUI main_ui;
	
	MainUI_constructor(&main_ui);
	
	/* TEST */
	ocr_font = MarkingFontTTFactory_getMarkingFontTT("OCR");


	main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, 0, 0);

	ticks1 = EsicGetTicks();
	while(looping) {
		Event system_event;

		Event_constructor(&system_event);

		/* System Top rectangle */
		//LcdDrawRectangle(0, 0, 319, 14, RGB_16B(240,240,240), RGB_16B(0,0,0));

		//TestDrawChar(ocr_font, 100, 100);

		TestDrawString(ocr_font, 50, 70, "SIC MARKING");
		//TestDrawString(ocr_font, 40, 95, "PauseEnSecondes");

		if(EsicPollEvent(&system_event)) {
			
			switch(system_event.type) {
			case EVENT_QUIT:
 				looping = (BOOL)!looping;
				break;

			/* Other events */

			default:
				WIDGET_VTABLE(&main_ui)->defaultProc(WIDGET(&main_ui), &system_event);
				//main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &system_event);
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

		//EsicDelay(15);
		LcdUpdate();

		Event_destructor(&system_event.object);

	}
	
	MainUI_destructor(&main_ui.e11ui.widget.object);

}

void TestDrawChar(PMarkingFontTT font, DWORD base_x, DWORD base_y, char ch) {
	DWORD i;

	ch -= ' ';

	for(i = 0; i < font->characters[ch].number_of_points; ++i) {
		if(font->characters[ch].coords[i].x1 >= 0) {
			LcdSetPixel(
				base_x + (font->characters[ch].coords[i].x1/7.0f),
				base_y + (((180) - font->characters[ch].coords[i].y1)/7.0f),
				0
			);
		}

		
		/*
		if(font->characters[ch].coords[i].x2 >= 0) {
			LcdSetPixel(
				base_x + (font->characters[ch].coords[i].x2/7.0),
				base_y + (((180) - font->characters[ch].coords[i].y2)/7.0),
				0
			);
		}
		*/
		

	}

}

void TestDrawString(PMarkingFontTT font, DWORD base_x, DWORD base_y, const char* string) {
	DWORD i;
	DWORD len = strlen(string);

	for(i = 0; i < len; ++i) {
		if(string[i] > ' ') {
			TestDrawChar(font, base_x, base_y, string[i]);
			base_x += (font->characters[string[i] - ' '].width/7.0);
		} else {
			base_x += 10;
		}
	}
}