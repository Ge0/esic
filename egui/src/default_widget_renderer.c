/**
 * \file default_widget_renderer.c
 */
#include <esic/egui/default_widget_renderer.h>
#include <esic/elcd/lcd.h>
#include <esic/egui/label.h>
#include <esic/egui/picture.h>
#include <esic/egui/checkbox.h>
#include <esic/egui/graphicsview/canvas.h>
/*#include <esic/egui/image.h>*/
#include <esic/egraphics/lcd_painter.h>
#include <esic/egraphics/shape_ptr.h>
#include <esic/eresources/raster_font_factory.h>

static const vtable_Object s_object_vtable = {
	DefaultWidgetRenderer_destructor,
	DefaultWidgetRenderer_clone,
	NULL,
	NULL
};

static const vtable_AbstractWidgetRenderer s_abstract_widget_renderer_vtable = {
	DefaultWidgetRenderer_paintLabel,
	DefaultWidgetRenderer_paintTextBox,
	DefaultWidgetRenderer_paintPicture,
	DefaultWidgetRenderer_paintCheckBox,
	DefaultWidgetRenderer_paintCanvas,
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
	ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawString(
	//real_self->painter->abstract_painter.vtable->drawString(
		ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
		//&real_self->painter->abstract_painter,
		base_x + label->widget.x,
		base_y + label->widget.y,
		label->widget.color,
		label->caption.data,
		"6x8.flcd"
	);

}

void DefaultWidgetRenderer_paintCheckBox(PAbstractWidgetRenderer self, PCheckBox checkbox, WORD base_x, WORD base_y) {
	/* Draw the rectangle */
	ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
		ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
		base_x + WIDGET(checkbox)->x,
		base_y + WIDGET(checkbox)->y,
		10,
		10,
		checkbox->is_focused ? RGB_16B(220,220,220) : RGB_16B(180,180,180),	/* Background color */
		RGB_16B(0,0,0)			/* Border color */
	);

	/* Draw another inner rectangle if the checkbox is checked */
	if(checkbox->is_checked) {
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			base_x + WIDGET(checkbox)->x + 2,
			base_y + WIDGET(checkbox)->y + 2,
			6,
			6,
			RGB_16B(0,255,0),		/* Background color */
			RGB_16B(0,255,0)			/* Border color */
		);
	}

}

void DefaultWidgetRenderer_paintTextBox(PAbstractWidgetRenderer self, PTextBox textbox, WORD base_x, WORD base_y) {
	PDefaultWidgetRenderer real_self = (PDefaultWidgetRenderer)self;
	//ZString visible_text;
	ZStringBuffer visible_text;
	WORD character_width = TEXTBOX_DEFAULT_WIDTH_CHARACTER;
	PRasterFont font6x8 = NULL;

	font6x8 = RasterFontFactory_getRasterFont("6x8.flcd"); /* Font to define dynamically later? */
	if(font6x8 != NULL) {
		character_width = font6x8->header.base_character_width;
	}

	/* Draw the surrounding rect with border */
	ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
	//real_self->painter->abstract_painter.vtable->drawRectangle(
		ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
		//&real_self->painter->abstract_painter,
		base_x + textbox->widget.x,
		base_y + textbox->widget.y,
		textbox->widget.width,
		textbox->widget.height + 4,
		textbox->is_focused ? textbox->focused_background_color : textbox->background_color,
		textbox->is_focused ? textbox->focused_border_color : textbox->border_color
	);
	
	/* Draw the only visible part of text */
	//ZString_constructor(&visible_text, "");
	ZStringBuffer_constructor(&visible_text);

	//ZString_subString(&textbox->text, textbox->text_offset, textbox->widget.width / character_width, &visible_text);
	ZStringBuffer_subString(&textbox->text, textbox->text_offset, textbox->widget.width / character_width, &visible_text); 

	/* Draw the string */
	ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawString(
		//real_self->painter->abstract_painter.vtable->drawString(
		ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
		//&real_self->painter->abstract_painter,
		base_x + textbox->widget.x + 2,
		base_y + textbox->widget.y + 2,
		RGB_16B(0,0,0),
		visible_text.data,
		"6x8.flcd"
	);

		if(textbox->draw_carret && textbox->is_focused) {
			/* Draw the carret */
			ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
				ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
				//real_self->painter->abstract_painter.vtable->drawRectangle(
				//&real_self->painter->abstract_painter,
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
			ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
				ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
				//real_self->painter->abstract_painter.vtable->drawRectangle(
				//&real_self->painter->abstract_painter,
				base_x + textbox->widget.x ,
				base_y + textbox->widget.y,
				2,
				textbox->widget.height+4,
				RGB_16B(20,20,20),
				RGB_16B(20,20,20)
			);
		}

		//if((WORD)(textbox->text_offset + (textbox->widget.width / character_width)) < textbox->text.size) {
		if((WORD)(textbox->text_offset + (textbox->widget.width / character_width)) < textbox->text.logical_size) {
			ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
				ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
				//real_self->painter->abstract_painter.vtable->drawRectangle(
				//&real_self->painter->abstract_painter,
				base_x + textbox->widget.x + textbox->widget.width-2 ,
				base_y + textbox->widget.y,
				2,
				textbox->widget.height+4,
				RGB_16B(20,20,20),
				RGB_16B(20,20,20)
			);
		}

	//ZString_destructor(OBJECT(&visible_text));
	ZStringBuffer_destructor(OBJECT(&visible_text));
}

void DefaultWidgetRenderer_paintPicture(PAbstractWidgetRenderer self, PPicture picture, WORD base_x, WORD base_y) {
	WORD border_color = picture->border_color;
	if(picture->is_focused) {
		//border_color ^= 0xFFFF;
		border_color = picture->border_color_hot;
	}
	
	if(WIDGET(picture)->is_hot) {
		border_color = picture->border_color_hot;
	}

	if(picture->icon != NULL) {
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawBuffer(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawBuffer(
			//&real_self->painter->abstract_painter,
			base_x + picture->widget.x,
			/*base_y*/0 + picture->widget.y,
			picture->icon->header.width,
			picture->icon->header.height,
			(WORD*)picture->icon->data
		);

		/* ... & the surronding rect */

		/* Top border */
		//LcdDrawRectangle(
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + picture->widget.x - picture->border_thickness,
			picture->widget.y - picture->border_thickness,
			picture->icon->header.width + picture->border_thickness,
			picture->border_thickness,
			border_color,
			border_color
		);

		/* Right border */
		//LcdDrawRectangle(
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + picture->widget.x + picture->icon->header.width,
			/*base_y*/0 + picture->widget.y - picture->border_thickness,
			picture->border_thickness,
			picture->icon->header.height + picture->border_thickness,
			border_color,
			border_color
		);

		/* Bottom border */
		//LcdDrawRectangle(
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + picture->widget.x,
			/*base_y*/0 + picture->widget.y + picture->icon->header.height,
			picture->icon->header.width + picture->border_thickness,
			picture->border_thickness,
			border_color,
			border_color
		);

		/* Left border */
		//LcdDrawRectangle(
		ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + picture->widget.x - picture->border_thickness,
			/*base_y*/ 0 + picture->widget.y,
			picture->border_thickness,
			picture->icon->header.height + picture->border_thickness,
			border_color,
			border_color
		);



	}

}

void DefaultWidgetRenderer_paintCanvas(PAbstractWidgetRenderer self, PCanvas canvas, WORD base_x, WORD base_y) {
	ClippingRegion saved_region = ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region;
	PListNode current_shape = NULL;

	/* Simply draw a rectangle */
	ABSTRACTPAINTER_VTABLE(DEFAULTWIDGETRENDERER(self)->painter)->drawRectangle(
			ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter),
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + canvas->widget.x,
			base_y + canvas->widget.y,
			canvas->widget.width,
			canvas->widget.height,
			RGB_16B(200,200,200),
			0
		);


	/* Test: define a clipping region for the painter */
	ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region.x1 = base_x + canvas->widget.x;
	ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region.y1 = base_y + canvas->widget.y;
	ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region.x2 = base_x + canvas->widget.x + canvas->widget.width;
	ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region.y2 = base_y + canvas->widget.y + canvas->widget.height;

	/* TODO: get each Shape of the model & paint it */
	if(canvas->scene != NULL) {
		current_shape = canvas->scene->shapes.head;
		
		while(current_shape != NULL) {
			PShape shape = SHAPEPTR(current_shape->data)->shape;

			shape->vtable->paint(shape, ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter));

			current_shape = current_shape->next;
		}

	}

	ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region = saved_region;
}

/*
void DefaultWidgetRenderer_paintImage(PImage self, WORD base_x, WORD base_y) {

	WORD i,j;


	LcdDrawRectangle(base_x + self->widget.x, base_y + self->widget.y, self->widget.width, self->widget.height, 0x0000, 0x8888);

	Lcd_drawString(base_x + self->widget.x + 2, base_y + self->widget.y + 2, 0xabcd, self->text.data);


	for ( i = 0; i < self->widget.height; i++)
	{
		for ( j = 0; j < self->widget.width; j++)
		{

			switch(self->bpp){
			case BPP1:

				LcdSetPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
				break;
			case BPP8:

				LcdSetPixel(j + self->widget.x, i + self->widget.y, self->rawBuffer[i * self->widget.width + j]); 
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
