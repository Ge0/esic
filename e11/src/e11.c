#include "e11.h"
#include <malloc.h>

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

#include "xml_ui_factory.h"

extern unsigned char image[HEIGHT][WIDTH];
extern PLcdFont _build_font(const char* font_name);


void e11(PAbstractSystem system) {
	PDefaultWidgetRenderer widget_renderer = NULL;

	Lcd_init(320, 240, system->vtable->getFrameBuffer(system), 0);

	/* FACTORY INIT */
	RasterFontFactory_init();
	RasterIconFactory_init();
	widget_renderer = NEW(widget_renderer, DefaultWidgetRenderer);
	SetDefaultWidgetRenderer((PAbstractWidgetRenderer)widget_renderer);

	//system->painter.raster_font = RasterFontFactory_getRasterFont("6x8.flcd");
	_e11_splashscreen(system);
	_e11_mainloop(system);
	
	DELETE(widget_renderer);

	/* FACTORY DESTROY */
	RasterIconFactory_destroy();
	RasterFontFactory_destroy();

	Lcd_destroy();
}



void _e11_splashscreen(PAbstractSystem system) {

}

void _e11_mainloop(PAbstractSystem system) {
	BOOL looping = TRUE;
	RasterIconHeader icon_hdr;
	BYTE* icon_buffer = NULL;
	PRasterIcon pen_icon = NULL,
		edit_icon = NULL,
		preview_icon = NULL,
		list_icon = NULL,
		settings_icon = NULL,
		ask_icon = NULL;

	/* TEST START */
	PWidget main_window;
	PRasterFont test_font = NULL;
	Image TTFfont;

	Lcd_setDrawingMode(LCD_OVER);

	/* Loading icons test */
	pen_icon      = RasterIconFactory_getRasterIcon("dotpen.ilcd");
	edit_icon     = RasterIconFactory_getRasterIcon("edition.ilcd");
	preview_icon  = RasterIconFactory_getRasterIcon("preview.ilcd");
	list_icon     = RasterIconFactory_getRasterIcon("list.ilcd");
	settings_icon = RasterIconFactory_getRasterIcon("settings.ilcd");
	ask_icon      = RasterIconFactory_getRasterIcon("ask.ilcd");

	/* TEST : DRAWING PICTURE */
	if(pen_icon != NULL) {
		Lcd_drawPicture(9, 208, pen_icon->header.width, pen_icon->header.height, (WORD*)pen_icon->data);
	}

	if(edit_icon != NULL) {
		Lcd_drawPicture(9+32+9+4+9, 208, edit_icon->header.width, edit_icon->header.height, (WORD*)edit_icon->data);
	}

	if(preview_icon != NULL) {
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9, 208, preview_icon->header.width, preview_icon->header.height, (WORD*)preview_icon->data);
		
	}

	if(list_icon != NULL) {
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9, 208, list_icon->header.width, list_icon->header.height, (WORD*)list_icon->data);
	}

	if(settings_icon != NULL) {
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208, settings_icon->header.width, settings_icon->header.height, (WORD*)settings_icon->data);
	}

	if(ask_icon != NULL) {
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208, ask_icon->header.width, ask_icon->header.height, (WORD*)ask_icon->data);
	}

	/* Applying the font */
	/*
	if(test_font != NULL) {
		Lcd_setFont(test_font);
	}
	*/

	/* END TEST */
	
	
	Lcd_drawRectangle(0, 0, 319, 14, RGB_16B(255,255,255), RGB_16B(0,0,0));

	/*
	Label_paint(&lbl.widget, 0, 0);
	TextBox_paint(&tbx.widget, 0, 0);
	*/

	//Lcd_drawTriangle(50, 100, 100, 50, 150, 214, RGB(255,255,255), RGB(255,0,255));
	//Image_paint(&TTFfont.widget);		
	
	main_window = XmlUiFactory_getUI("ui");
	if(main_window != NULL) {
		SicHeapDump();
		main_window->vtable->paint(main_window, 0, 16);
		SicHeapDump();
	}
	while(looping) {
		Event systemEvent;
		if(system->vtable->pollEvent(system, &systemEvent)) {
			switch(systemEvent.type) {
			case EVENT_QUIT:
				looping = !looping;
				break;
			}
		}

		

		system->vtable->update(system);
		Lcd_update();

		system->vtable->delay(system, 33);		
	}
	
	if(main_window != NULL) {
		DELETE(main_window);
	}

	/*
	Label_destructor((PObject)&lbl);
	TextBox_destructor((PObject)&tbx);
	*/


	/*
	if(testFont != NULL) {
		DELETE(testFont);
	}
	*/

	

	SicHeapDump();
}
