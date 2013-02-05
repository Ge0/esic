#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/label.h>

void DefaultWidgetRenderer_paintLabel(PLabel self) {
	Lcd_drawString(self->widget.x, self->widget.y, 0xabcd, self->caption.data);
}

void DefaultWidgetRenderer_paintTextBox(PTextBox self) {
	/* Draw the surrounding rect */
	Lcd_drawRectangle(self->widget.x, self->widget.y, self->widget.width, 12, 0x0000, 0x8888);

	/* Draw the content inside */
	Lcd_drawString(self->widget.x + 2, self->widget.y + 2, 0xabcd, self->text.data);
}