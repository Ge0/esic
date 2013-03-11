#include "e11.h"
#include <stdlib.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>
#include <esic/egui/image.h>
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
	widget_renderer = NEW(widget_renderer, DefaultWidgetRenderer);
	SetDefaultWidgetRenderer((PAbstractWidgetRenderer)widget_renderer);

	/* Splashscreen */
	_e11_splashscreen(system);

	/* And main loop */
	_e11_mainloop(system);
	
	/* Free the resources */
	DELETE(widget_renderer);

	/* FACTORY DESTROY */
	RasterIconFactory_destroy();
	RasterFontFactory_destroy();

	SicHeapDump();
}



void _e11_splashscreen(PAbstractSystem system) {

}

void _e11_mainloop(PAbstractSystem system) {
	BOOL looping = 1;
	DWORD ticks1, ticks2;
	/* TEST START */
	PRasterFont test_font = NULL;
	SzString s1, s2;
	

	/* Test GR */
	MainUI main_ui;

	MainUI_constructor(&main_ui);

	SzString_constructor(&s1, "Hello world");
	SzString_constructor(&s2, "");

	SzString_subString(&s1, -4, 0, &s2);

	printf("%s", s2.data);
	
	/* System Top rectangle */
	Lcd_drawRectangle(0, 0, 319, 14, RGB_16B(240,240,240), RGB_16B(0,0,0));

	//main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, 0, 0);


	
	ticks1 = system->vtable->getTicks(system);
	while(looping) {
		Event systemEvent;
		if(system->vtable->pollEvent(system, &systemEvent)) {
			switch(systemEvent.type) {
			case EVENT_QUIT:
 				looping = !looping;
				break;
			case EVENT_KEYBOARD_KDOWN:

				/* To remove later */
				if(systemEvent.real_event.keyboard_event.code == KEY_F12) {
					looping = !looping;
				}
				main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &systemEvent);
				break;
			default:
				main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &systemEvent);
			}
		}

		/* test */
		ticks2 = system->vtable->getTicks(system);
		if(ticks2 - ticks1 >= 550) {
			/* One second elapsed */
			Event timer_event;
			timer_event.type = EVENT_TIMER;
			timer_event.real_event.timer_event.id = 1;
			singleton_system()->vtable->enqueueEvent(singleton_system(), &timer_event);
			ticks1 = ticks2;
		}
		

		system->vtable->update(system);
		system->vtable->delay(system, 15);
		Lcd_update();

	}
	

	MainUI_destructor(&main_ui.e11ui.widget.object);

	SzString_destructor(&s1.object);
	SzString_destructor(&s2.object);

}
