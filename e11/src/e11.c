#include "e11.h"
#include <stdlib.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>

#include <esic/eapi/heap.h>

/* TEST START */
#include <esic/test/testFreeType.h>
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>
#include <esic/egui/image.h>
#include <esic/etools/szstring.h>
#include <esic/eapi/raster_font_factory.h>
#include <esic/eapi/raster_icon_factory.h>
#include <esic/eapi/raster_icon_header.h>
#include <esic/egui/default_widget_renderer.h>
#include <fatfs/ff.h>
/* TEST END */

/* OTHER TEST START */
#include <ui/main_ui.h>

#include "xml_ui_factory.h"

extern unsigned char image[HEIGHT][WIDTH];
extern PLcdFont _build_font(const char* font_name);


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

	/* TEST START */
	PWidget main_window;
	PRasterFont test_font = NULL;

	/* Test GR */
	MainUI main_ui;

	MainUI_constructor(&main_ui);
	
	/* System Top rectangle */
	Lcd_drawRectangle(0, 0, 319, 14, RGB_16B(240,240,240), RGB_16B(0,0,0));


	main_ui.e11ui.widget.vtable->paint(&main_ui.e11ui.widget, 0, 16);
	while(looping) {
		Event systemEvent;
		if(system->vtable->pollEvent(system, &systemEvent)) {
			switch(systemEvent.type) {
			case EVENT_QUIT:
				looping = !looping;
				break;

			default:
				main_ui.e11ui.widget.vtable->defaultProc(&main_ui.e11ui.widget, &systemEvent);
			}
		}

		

		system->vtable->update(system);
		Lcd_update();

		system->vtable->delay(system, 33);		
	}
	

	MainUI_destructor(&main_ui.e11ui.widget.object);

}
