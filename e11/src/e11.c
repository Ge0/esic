#include "e11.h"
#include <stdlib.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>
#include <esic/eapi/system.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/eapi/raster_font_factory.h>
#include <esic/eapi/raster_icon_factory.h>
#include <esic/eapi/raster_icon_header.h>
#include <esic/egui/default_widget_renderer.h>
#include <esic/etools/zstring.h>
#include <fatfs/ff.h>
/* TEST END */

/* OTHER TEST START */
#include <ui/main_ui.h>

#include "xml_ui_factory.h"

void e11() {
	PDefaultWidgetRenderer widget_renderer = NULL;

	
	/* LCD INIT */
	LcdInit(320, 240, 16, LCD_DOUBLEBUF);
	LcdFill(RGB_16B(240,240,240));

	/* FACTORY INIT */
	RasterFontFactory_init();
	RasterIconFactory_init();


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
	RasterIconFactory_destroy();
	RasterFontFactory_destroy();

	SicHeapDump();
}



void _e11_splashscreen() {

}

void _e11_mainloop() {
	BOOL looping = 1;
	DWORD ticks1, ticks2;

	MainUI main_ui;

	MainUI_constructor(&main_ui);
	
	

	main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, 0, 0);

	ticks1 = EsicGetTicks();
	while(looping) {
		Event system_event;

		Event_constructor(&system_event);

		/* System Top rectangle */
		LcdDrawRectangle(0, 0, 319, 14, RGB_16B(240,240,240), RGB_16B(0,0,0));

		if(EsicPollEvent(&system_event)) {

			switch(system_event.type) {
			case EVENT_QUIT:
 				looping = !looping;
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
