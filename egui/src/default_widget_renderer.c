#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/label.h>
#include <esic/egui/image.h>

void DefaultWidgetRenderer_paintLabel(PLabel self) {
	Lcd_drawString(self->widget.x, self->widget.y, 0xabcd, self->caption.data);
}

void DefaultWidgetRenderer_paintTextBox(PTextBox self) {
	/* Draw the surrounding rect */
	Lcd_drawRectangle(self->widget.x, self->widget.y, self->widget.width, 12, RGB_16B(200,200,200), RGB_16B(0,0,0));

	/* Draw the content inside */
	Lcd_drawString(self->widget.x + 2, self->widget.y + 2, RGB_16B(0,0,0), self->text.data);
}

void DefaultWidgetRenderer_paintImage(PImage self) {

	WORD i,j;

	/* Draw the surrounding rect */
	Lcd_drawRectangle(self->widget.x, self->widget.y, self->widget.width, self->widget.height, 0x0000, 0x8888);

	/* Draw the content inside */
	Lcd_drawString(self->widget.x + 2, self->widget.y + 2, 0xabcd, self->text.data);

	/* Draw the image */	
	for ( i = 0; i < self->widget.height; i++)
	{
		for ( j = 0; j < self->widget.width; j++)
		{
			//*self->rawBuffer++ = self->rawBuffer[i * self->widget.width + j];
			/* get the pixel color */
			switch(self->bpp){
			case BPP1:
				/* no color, used default to black and white */
				Lcd_setPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
				break;
			case BPP8:
				/* 8 bits per pixel, using grey level */
				Lcd_setPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
				break;
			case BPP16:
				/* use of a palette */
				break;
			case BPP24:
				/* use RGB directly */
				break;

			}


		}
	}
}