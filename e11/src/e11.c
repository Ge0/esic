#include "e11.h"
#include <malloc.h>

#include <esic/elcd/lcd.h>
#include <esic/eapi/event.h>

/* TEST START */
#include <esic/egui/label.h>
#include <esic/egui/textbox.h>
#include <esic/etools/szstring.h>
#include <esic/elcd/lcd_font_factory.h>
/* TEST END */


void e11(PAbstractSystem system) {
	Lcd_init(480, 272, system->vtable->getFrameBuffer(system), 0);
	_e11_splashscreen(system);
	_e11_mainloop(system);
	Lcd_destroy();
}

void _e11_splashscreen(PAbstractSystem system) {

}

void _e11_mainloop(PAbstractSystem system) {
	PEvent systemEvent = NULL;
	BOOL looping = TRUE;
	

	/* TEST START */
	PLcdFont testFont = NULL;
	Label lbl;
	TextBox tbx;

	testFont = build_font("6x8.flcd");
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
	/* TEST END */

	Lcd_setDrawingMode(LCD_OVER);
	

	while(looping) {
		if(system->vtable->pollEvent(system, &systemEvent)) {
			switch(systemEvent->type) {
			case EVENT_QUIT:
				looping = !looping;
				break;
			}
			DELETE(systemEvent);
		}

		Lcd_drawRectangle(0, 0, 319, 14, 0x8888, 0xffff);
		Label_paint(&lbl.widget);
		TextBox_paint(&tbx.widget);
		Lcd_drawTriangle(50, 100, 100, 50, 150, 214, 0x8888, 0xefa2);
		
		system->vtable->update(system);
		Lcd_update();

		system->vtable->delay(system, 33);
		
	}

	Label_destructor((PObject)&lbl);
	TextBox_destructor((PObject)&tbx);
	DELETE(testFont);
}