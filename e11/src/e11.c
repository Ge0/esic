#include "e11.h"
#include <stdlib.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>
#include <esic/eapi/system.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/etools/szstring.h>
#include <esic/eapi/raster_font_factory.h>
#include <esic/eapi/raster_icon_factory.h>
#include <esic/eapi/raster_icon_header.h>
#include <esic/egui/default_widget_renderer.h>
#include <esic/etools/szstring.h>
#include <fatfs/ff.h>
/* TEST END */

/* OTHER TEST START */
#include <ui/main_ui.h>

#include "xml_ui_factory.h"

void e11(PAbstractSystem system) {
	PDefaultWidgetRenderer widget_renderer = NULL;

 

	/* FACTORY INIT */
	RasterFontFactory_init();
	RasterIconFactory_init();

	Lcd_setDrawingMode(LCD_OVER);

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
	
	/* System Top rectangle */
	Lcd_drawRectangle(0, 0, 319, 14, RGB_16B(240,240,240), RGB_16B(0,0,0));

	main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, 0, 0);

	ticks1 = EsicGetTicks();
	while(looping) {
		Event system_event;

		Event_constructor(&system_event);

		if(EsicPollEvent(&system_event)) {
			switch(system_event.type) {
			case EVENT_QUIT:
 				looping = !looping;
				break;
			case EVENT_KEYBOARD_KDOWN:

				/* To remove later */
				if(system_event.real_event.keyboard_event.code == KEY_F12) {
					looping = !looping;
				}
				main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &system_event);
				break;
			default:
				main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &system_event);
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
		
		EsicUpdate();
		//EsicDelay(15);
		Lcd_update();

		Event_destructor(&system_event.object);

	}
	
	MainUI_destructor(&main_ui.e11ui.widget.object);

}
