/**
 * \file default_widget_renderer.c
 */
#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/label.h>
#include <esic/egui/picture.h>
#include <esic/egui/image.h>
#include <esic/elcd/lcd_painter.h>
#include <esic/eapi/raster_font_factory.h>

static const vtable_Object s_object_vtable = {
	DefaultWidgetRenderer_destructor,
	DefaultWidgetRenderer_clone,
	NULL,
	NULL
};

static const vtable_AbstractWidgetRenderer s_abstract_widget_renderer_vtable = {
	DefaultWidgetRenderer_paintLabel,
	DefaultWidgetRenderer_paintTextBox,
	DefaultWidgetRenderer_paintPicture
};

static PAbstractWidgetRenderer s_default_renderer;

PDefaultWidgetRenderer DefaultWidgetRenderer_constructor(PDefaultWidgetRenderer self) {
	self->abstract_widget_renderer.object.size = sizeof(DefaultWidgetRenderer);
	self->abstract_widget_renderer.object.vtable = &s_object_vtable;
	self->abstract_widget_renderer.vtable = &s_abstract_widget_renderer_vtable;
	self->painter = NEW(self->painter, LcdPainter);
	self->painter->abstract_painter.raster_font = RasterFontFactory_getRasterFont("6x8.flcd");

	return self;
}

void DefaultWidgetRenderer_destructor(PObject self) {
	PDefaultWidgetRenderer real_self = (PDefaultWidgetRenderer)self;

	DELETE(real_self->painter);
}

PObject DefaultWidgetRenderer_clone(PObject self, PObject dst) {
	/* TODO. */

	return dst;
}

void SetDefaultWidgetRenderer(PAbstractWidgetRenderer renderer) {
	s_default_renderer = renderer;
}

PAbstractWidgetRenderer GetDefaultWidgetRenderer() {
	return s_default_renderer;
}

void DefaultWidgetRenderer_paintLabel(PAbstractWidgetRenderer self, PLabel label, WORD base_x, WORD base_y) {
	PDefaultWidgetRenderer real_self = (PDefaultWidgetRenderer)self;

	/* Simply draw the caption */
	real_self->painter->abstract_painter.vtable->drawString(
		&real_self->painter->abstract_painter,
		base_x + label->widget.x,
		base_y + label->widget.y,
		label->widget.color,
		label->caption.data
	);

}

void DefaultWidgetRenderer_paintTextBox(PAbstractWidgetRenderer self, PTextBox textbox, WORD base_x, WORD base_y) {
	PDefaultWidgetRenderer real_self = (PDefaultWidgetRenderer)self;
	SzString visible_text;
	WORD character_width = TEXTBOX_DEFAULT_WIDTH_CHARACTER;
	PRasterFont font6x8 = NULL;

	font6x8 = RasterFontFactory_getRasterFont("6x8.flcd"); /* Font to define dynamically later? */
	if(font6x8 != NULL) {
		character_width = font6x8->header.base_character_width;
	}

	/* Draw the surrounding rect with border */
	real_self->painter->abstract_painter.vtable->drawRectangle(
		&real_self->painter->abstract_painter,
		base_x + textbox->widget.x,
		base_y + textbox->widget.y,
		textbox->widget.width,
		textbox->widget.height + 4,
		textbox->is_focused ? textbox->focused_background_color : textbox->background_color,
		textbox->is_focused ? textbox->focused_border_color : textbox->border_color
	);
	
	/* Draw the only visible part of text */
	SzString_constructor(&visible_text, "");

	SzString_subString(&textbox->text, textbox->text_offset, textbox->widget.width / character_width, &visible_text); 

	/* Draw the string & the carret at its physical position */
		real_self->painter->abstract_painter.vtable->drawString(
			&real_self->painter->abstract_painter,
			base_x + textbox->widget.x + 2,
			base_y + textbox->widget.y + 2,
			RGB_16B(0,0,0),
			visible_text.data
		);

		if(textbox->draw_carret && textbox->is_focused) {
			real_self->painter->abstract_painter.vtable->drawRectangle(
				&real_self->painter->abstract_painter,
				base_x + textbox->widget.x + 2 + ((textbox->carret_position - textbox->text_offset) * character_width) ,
				base_y + textbox->widget.y + 2,
				1,
				textbox->widget.height,
				RGB_16B(0,0,0),
				RGB_16B(0,0,0)
			);
		}

		/* Draw indicators at the beginning if there are characters non printed */
		if(textbox->text_offset) {
			real_self->painter->abstract_painter.vtable->drawRectangle(
				&real_self->painter->abstract_painter,
				base_x + textbox->widget.x ,
				base_y + textbox->widget.y,
				2,
				textbox->widget.height+4,
				RGB_16B(20,20,20),
				RGB_16B(20,20,20)
			);
		}

		if((WORD)(textbox->text_offset + (textbox->widget.width / character_width)) < textbox->text.size) {
			real_self->painter->abstract_painter.vtable->drawRectangle(
				&real_self->painter->abstract_painter,
				base_x + textbox->widget.x + textbox->widget.width-2 ,
				base_y + textbox->widget.y,
				2,
				textbox->widget.height+4,
				RGB_16B(20,20,20),
				RGB_16B(20,20,20)
			);
		}

	SzString_destructor(&visible_text.object);
}

void DefaultWidgetRenderer_paintPicture(PAbstractWidgetRenderer self, PPicture picture, WORD base_x, WORD base_y) {
	PDefaultWidgetRenderer real_self = (PDefaultWidgetRenderer)self;

	if(picture->icon != NULL) {
		real_self->painter->abstract_painter.vtable->drawBuffer(
			&real_self->painter->abstract_painter,
			base_x + picture->widget.x,
			base_y + picture->widget.y,
			picture->icon->header.width,
			picture->icon->header.height,
			(WORD*)picture->icon->data
		);
	}

}

/*
void DefaultWidgetRenderer_paintImage(PImage self, WORD base_x, WORD base_y) {

	WORD i,j;


	Lcd_drawRectangle(base_x + self->widget.x, base_y + self->widget.y, self->widget.width, self->widget.height, 0x0000, 0x8888);

	Lcd_drawString(base_x + self->widget.x + 2, base_y + self->widget.y + 2, 0xabcd, self->text.data);


	for ( i = 0; i < self->widget.height; i++)
	{
		for ( j = 0; j < self->widget.width; j++)
		{

			switch(self->bpp){
			case BPP1:

				Lcd_setPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
				break;
			case BPP8:

				Lcd_setPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
				break;
			case BPP16:

				break;
			case BPP24:

				break;

			}


		}
	}
}
*/