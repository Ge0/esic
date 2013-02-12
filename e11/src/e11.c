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
#include <esic/elcd/lcd_font_factory.h>
#include <esic/elcd/lcd_icon_factory.h>
#include <esic/elcd/lcd_icon_header.h>
#include <fatfs/ff.h>
/* TEST END */

#include "xml_ui_factory.h"

extern unsigned char image[HEIGHT][WIDTH];
extern PLcdFont _build_font(const char* font_name);


void e11(PAbstractSystem system) {
	Lcd_init(320, 240, system->vtable->getFrameBuffer(system), 0);
	_e11_splashscreen(system);
	_e11_mainloop(system);
	Lcd_destroy();
}



void _e11_splashscreen(PAbstractSystem system) {

}

void _e11_mainloop(PAbstractSystem system) {
	BOOL looping = TRUE;
	LcdIconHeader icon_hdr;
	BYTE* icon_buffer = NULL;
	PLcdIcon pen_icon = NULL,
		edit_icon = NULL,
		preview_icon = NULL,
		list_icon = NULL,
		settings_icon = NULL,
		ask_icon = NULL;

	/* TEST START */
	PWidget main_window;
	PLcdFont test_font = NULL;
	Image TTFfont;


	/* FACTORY INIT */
	LcdFontFactory_init();
	LcdIconFactory_init();

	Lcd_setDrawingMode(LCD_OVER);

	/* Loading font test */
	test_font = LcdFontFactory_getLcdFont("6x8.flcd");

	/* Loading icons test */
	pen_icon      = LcdIconFactory_getLcdIcon("dotpen.ilcd");
	edit_icon     = LcdIconFactory_getLcdIcon("edition.ilcd");
	preview_icon  = LcdIconFactory_getLcdIcon("preview.ilcd");
	list_icon     = LcdIconFactory_getLcdIcon("list.ilcd");
	settings_icon = LcdIconFactory_getLcdIcon("settings.ilcd");
	ask_icon      = LcdIconFactory_getLcdIcon("ask.ilcd");

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
	if(test_font != NULL) {
		Lcd_setFont(test_font);
	}

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
	
	DELETE(main_window);

	/*
	Label_destructor((PObject)&lbl);
	TextBox_destructor((PObject)&tbx);
	*/


	/*
	if(testFont != NULL) {
		DELETE(testFont);
	}
	*/

	/* FACTORY DESTROY */
	LcdIconFactory_destroy();
	LcdFontFactory_destroy();

	SicHeapDump();
}
