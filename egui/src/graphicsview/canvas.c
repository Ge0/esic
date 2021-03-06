#include <esic/egraphics/shape_ptr.h>
#include <esic/eapi/system.h>
#include <esic/egui/graphicsview/canvas.h>
#include <esic/egraphics/painter.h>
//#include <esic/egui/default_widget_renderer.h>

VTABLE_START(Object) {
	Canvas_destructor,
	Canvas_clone,
	Canvas_equalsTo,
	Canvas_hash,
	Canvas_type
};

VTABLE_START(Widget) {
	Canvas_defaultProc,
	Canvas_paint,
	Widget_handleWidgetEvent
};

VTABLE_START(Observer) {
	Canvas_update
};

static void _draw_vertical_scrollbar(PCanvas self, PPainter painter, WORD base_x, WORD base_y);
static void _draw_horizontal_scrollbar(PCanvas self, PPainter painter, WORD base_x, WORD base_y);
static void _draw_top_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y);
static void _draw_bottom_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y);
static void _draw_left_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y);
static void _draw_right_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y);


PCanvas Canvas_constructor(PCanvas self) {
	Widget_constructor(WIDGET(self));
	//Observer_constructor(&self->observer);

	OBJECT(self)->size = sizeof(Canvas);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	

	/* No model is attached */
	/* Points to the Object base instance of self for virtual inheritance */
	self->observer.object = OBJECT(self); 
	self->observer.vtable = VTABLE_POINTER(Observer);
	//Observer_constructor(&self->observer);
	self->scene = NULL;

	/* Widget properties */
	WIDGET(self)->vtable = VTABLE_POINTER(Widget);
	self->widget.is_focusable = FALSE;

	/* Canvas properties */
	self->scene = NULL;

	/* TEST */
	self->x_offset = 0;
	self->y_offset = 0;

	return self;
}

void Canvas_destructor(PObject self) {
	Widget_destructor(self);
}

PObject Canvas_clone(PObject self, PObject dst) {

	/* Calling parent function */
	Widget_clone(self, dst);

	CANVAS(dst)->observer = CANVAS(self)->observer;
	//ZStringBuffer_clone(&real_self->text.object, &real_dst->text.object);

	return self;
}

BOOL Canvas_equalsTo(PObject self, PObject dst) {
	/* TODO. */

	return FALSE;
}

DWORD Canvas_hash(PObject self) {
	/* TODO. */

	return 0;
}

DWORD Canvas_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Canvas");
	}

	return s_hash;
}


void Canvas_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	PListNode current_shape = NULL;
	ClipPainter saved_region = painter->clip;
	ClipPainter logical_region = { 0, 0, INT32_MIN,INT32_MIN };

	/* Simply draw a rectangle */
	//painter->color = RGB_16B(200,200,200);
	Painter_fillRectangle(
		painter,
		base_x + self->x,
		base_y + self->y,
		self->width,
		self->height,
		RGB_16B(200,200,200)
	);

	/* Border */
	Painter_drawRectangle(
			painter,
			base_x + self->x,
			base_y + self->y,
			self->width,
			self->height,
			RGB_16B(0,0,0)
		);

	/* Test: define a clipping region for the painter */
	painter->clip.x      = base_x + self->x;
	painter->clip.y      = base_y + self->y;
	painter->clip.width  = self->width;
	painter->clip.height = self->height;

	/* TODO: get each Shape of the model & paint it */
	if(CANVAS(self)->scene != NULL) {
		current_shape = CANVAS(self)->scene->shapes.head;
		
		while(current_shape != NULL) {

			SDWORD x_offset_shape = 0;
			SDWORD y_offset_shape = 0;

			PShape shape = SHAPEPTR(current_shape->data)->shape;

			/* Update the clipping region of the model, for scrolling purpose */
			if(shape->position.x > logical_region.width) {
				logical_region.width = shape->position.x;
			}

			if(shape->position.y > logical_region.height) {
				logical_region.height = shape->position.y;
			}

			x_offset_shape = base_x + self->x  - CANVAS(self)->x_offset;
			y_offset_shape = base_y + self->y  - CANVAS(self)->y_offset;


			// Physical translation so the shapes can be drawn
			// TODO: zoom support... But may be tough for pixels
			shape->vtable->translateTo(shape, x_offset_shape, y_offset_shape);

			shape->vtable->paint(shape, painter);

			shape->vtable->translateTo(shape, -x_offset_shape, -y_offset_shape);

			current_shape = current_shape->next;
		}

	}

	/* Test: draw scrollbars */
	
	/* Enlarge painter clipping region */
	painter->clip.width += 10;
	painter->clip.height += 10;

	_draw_horizontal_scrollbar(CANVAS(self), painter, base_x, base_y);
	_draw_vertical_scrollbar(CANVAS(self), painter, base_x, base_y);
	

	painter->clip = saved_region;

	
}

static void _draw_horizontal_scrollbar(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {

	//painter->color = RGB_16B(240,240,230);
	Painter_fillRectangle(
		painter,
		base_x + WIDGET(self)->x,
		base_y + WIDGET(self)->y + WIDGET(self)->height -1,
		WIDGET(self)->width,
		11,
		RGB_16B(240,240,230)
	);

	
	//painter->color = RGB_16B(0,0,0);
	Painter_drawRectangle(
		painter,
		base_x + WIDGET(self)->x,
		base_y + WIDGET(self)->y + WIDGET(self)->height -1,
		WIDGET(self)->width,
		11,
		RGB_16B(0,0,0)
	);

	_draw_left_scroll_arrow(self, painter, base_x, base_y);
	_draw_right_scroll_arrow(self, painter, base_x, base_y);

}

static void _draw_left_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {
	DWORD i = 0;
	WORD arrow_base_y = 4;
	WORD arrow_base_x = 2;

	for(i = 0; i < 4; i++) {
		Painter_drawLine(
			painter,
			base_x + WIDGET(self)->x + base_x + arrow_base_x + i,
			base_y + WIDGET(self)->y + WIDGET(self)->height + arrow_base_y + i,
			base_x + WIDGET(self)->x + base_x + arrow_base_x + i,
			base_y + WIDGET(self)->y + WIDGET(self)->height + arrow_base_y - i,
			RGB_16B(0,0,0)
		);

	}
}

static void _draw_right_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {
	DWORD i = 0;
	WORD arrow_base_y = 4;
	WORD arrow_base_x = WIDGET(self)->width - 3;

	for(i = 0; i < 4; i++) {
		Painter_drawLine(
			painter,
			base_x + WIDGET(self)->x + base_x + arrow_base_x - i,
			base_y + WIDGET(self)->y + WIDGET(self)->height + arrow_base_y + i,
			base_x + WIDGET(self)->x + base_x + arrow_base_x - i,
			base_y + WIDGET(self)->y + WIDGET(self)->height + arrow_base_y - i,
			RGB_16B(0,0,0)
		);

	}
}



static void _draw_vertical_scrollbar(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {
	//painter->color = RGB_16B(240,240,230);
	Painter_fillRectangle(
		painter,
		base_x + WIDGET(self)->x + WIDGET(self)->width - 1,
		base_y + WIDGET(self)->y,
		11,
		WIDGET(self)->height,
		RGB_16B(240,240,230)
	);

	//painter->color = RGB_16B(0,0,0);
	Painter_drawRectangle(
		painter,
		base_x + WIDGET(self)->x + WIDGET(self)->width - 1,
		base_y + WIDGET(self)->y,
		11,
		WIDGET(self)->height,
		RGB_16B(0,0,0)
	);

	_draw_top_scroll_arrow(self, painter, base_x, base_y);
	_draw_bottom_scroll_arrow(self, painter, base_x, base_y);
}


static void _draw_top_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {
	// Draw top arrow
	WORD arrow_base_y = 2;
	WORD arrow_base_x = 4;
	DWORD i = 0;

	for(i = 0; i < 4; i++) {
		Painter_drawLine(
			painter,
			base_x + WIDGET(self)->x + WIDGET(self)->width + arrow_base_x +  i,
			base_y + WIDGET(self)->y + arrow_base_y + i,
			base_x + WIDGET(self)->x + WIDGET(self)->width + arrow_base_x - i,
			base_y + WIDGET(self)->y + arrow_base_y + i,
			RGB_16B(0,0,0)
		);
	}

}

static void _draw_bottom_scroll_arrow(PCanvas self, PPainter painter, WORD base_x, WORD base_y) {
	// Draw top arrow
	WORD arrow_base_y = WIDGET(self)->height - 3;
	WORD arrow_base_x = 4;
	SDWORD i = 0;

	for(i = 3; i >= 0; i--) {
		Painter_drawLine(
			painter,
			base_x + WIDGET(self)->x + WIDGET(self)->width + arrow_base_x + i,
			base_y + WIDGET(self)->y + arrow_base_y - i,
			base_x + WIDGET(self)->x + WIDGET(self)->width + arrow_base_x - i,
			base_y + WIDGET(self)->y + arrow_base_y - i,
			RGB_16B(0,0,0)
		);
	}

}

DWORD Canvas_defaultProc(PWidget self, const PEvent system_event) {
	return Widget_defaultProc(self, system_event);
}

void Canvas_update(PObserver self, PObject data) {
	Event widget_event;
	/* Do we need data? */
	data = data;

	Event_constructor(&widget_event);
	/* Send a PAINT message to repaint the widget with model's data? */
	widget_event.type = EVENT_WIDGET;
	widget_event.real_event.widget_event.id = WIDGET(self->object)->id;
	widget_event.real_event.widget_event.type = WE_PAINT;

	EsicPushEvent(&widget_event);

	Event_destructor(OBJECT(&widget_event));
}

void Canvas_setGraphicsScene(PCanvas self, PGraphicsScene scene) {

	/* If there is another scene, unsuscribe from it */
	if(self->scene != NULL) {
		Observable_removeObserver(OBSERVABLE(self->scene), OBSERVER(&self->observer));
	}

	self->scene = scene;
	Observable_addObserver(OBSERVABLE(self->scene), OBSERVER(&self->observer));

}

void Canvas_scrollLeft(PCanvas self, SDWORD offset) {
	if(self->x_offset >= 10) {
		self->x_offset -= 10;
	}
}

void Canvas_scrollRight(PCanvas self, SDWORD offset) {
	self->x_offset += 10;
}


void Canvas_scrollUp(PCanvas self, SDWORD offset) {
	if(self->y_offset >= 10) {
		self->y_offset -= 10;
	}
}


void Canvas_scrollDown(PCanvas self, SDWORD offset) {
	self->y_offset += 10;
}

