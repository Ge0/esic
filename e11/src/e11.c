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
	FIL icon_file;
	LcdIconHeader icon_hdr;
	BYTE* icon_buffer = NULL;

	
	

	/* TEST START */
	PWidget mainWindow;
	PLcdFont testFont = NULL;
	Label lbl;
	TextBox tbx;
	Image TTFfont;


	/* FACTORY INIT */
	LcdFontFactory_init();

	/* UI FACTORY TEST */
	SicHeapDump();
	mainWindow = XmlUiFactory_getUI("ui");
	if(mainWindow != NULL) {
		SicHeapDump();
		DELETE(mainWindow);
		SicHeapDump();
	}
	

	/*
	testFont = LcdFontFactory_getLcdFont("6x8.flcd");
	//testFont = _build_font("6x8.flcd");
	Label_constructor(&lbl);
	TextBox_constructor(&tbx);

	lbl.widget.x = 20;
	lbl.widget.y = 100;
	SzString_setData(&lbl.caption, "Enter your name: ");

	tbx.widget.x     = 120;
	tbx.widget.y     = 98;
	tbx.widget.width = 70;
	SzString_setData(&tbx.text, "Geoffrey");

	Lcd_setFont(testFont);
	*/

	/* Test ilcd (image lcd) */
	if(f_open(&icon_file, (const TCHAR*)"system/icons/chemistry.ilcd", FA_READ) == FR_OK) {
		UINT br;
		f_read(&icon_file, &icon_hdr, sizeof(LcdIconHeader), &br);
		icon_buffer = (BYTE*)SicAlloc(icon_hdr.height * icon_hdr.width * sizeof(WORD));
		assert(icon_buffer != NULL);
		f_read(&icon_file, icon_buffer, icon_hdr.height * icon_hdr.width * sizeof(WORD), &br);
	}
	
	
	/* test TTF font */
	/*
	testLoadFreeType();		//create a row image in &image
	Image_constructor(&TTFfont);
	createRawImageFrom2DBuffer(&TTFfont, (const char*)&image, HEIGHT, WIDTH, BPP1);		
	TTFfont.widget.x  = 20;
	TTFfont.widget.y  = 125;	
	TTFfont.bpp = 1;
	TTFfont.palette = 16;	
	TTFfont.widget.width = WIDTH;
	TTFfont.widget.height = HEIGHT;
	SzString_setData(&TTFfont.text, "raster from TTF:");
	*/
	/* TEST END */

	Lcd_setDrawingMode(LCD_OVER);

	/* TEST : DRAWING PICTURE */
	if(icon_buffer != NULL) {
		Lcd_drawPicture(9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);

		Lcd_drawPicture(9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
		Lcd_drawPicture(9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9+32+9+4+9, 208-36, icon_hdr.width, icon_hdr.height, (WORD*)icon_buffer);
	}
	/* END TEST */
	
	
	Lcd_drawRectangle(0, 0, 319, 14, RGB_16B(255,255,255), RGB_16B(0,0,0));

	/*
	Label_paint(&lbl.widget, 0, 0);
	TextBox_paint(&tbx.widget, 0, 0);
	*/

	//Lcd_drawTriangle(50, 100, 100, 50, 150, 214, RGB(255,255,255), RGB(255,0,255));
	//Image_paint(&TTFfont.widget);		
	
	mainWindow = XmlUiFactory_getUI("ui");
	if(mainWindow != NULL) {
		SicHeapDump();
		mainWindow->vtable->paint(mainWindow, 0, 16);
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
	
	DELETE(mainWindow);

	/*
	Label_destructor((PObject)&lbl);
	TextBox_destructor((PObject)&tbx);
	*/


	/*
	if(testFont != NULL) {
		DELETE(testFont);
	}
	*/


	if(icon_buffer != NULL) {
		SicFree(icon_buffer);
	}

	/* FACTORY DESTROY */
	LcdFontFactory_destroy();

	SicHeapDump();
}
