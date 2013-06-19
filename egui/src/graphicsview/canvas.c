#include <esic/egraphics/shape_ptr.h>
#include <esic/eapi/system.h>
#include <esic/egui/graphicsview/canvas.h>
#include <esic/egui/default_widget_renderer.h>

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) Canvas_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Widget) {
/*
#define WIDGET_VFUNCTION(return_type, function_name, arguments) Canvas_##function_name,
	WIDGET_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
*/
	Canvas_defaultProc,
	Canvas_paint,
	Widget_handleWidgetEvent
};

VTABLE_START(Observer) {
	Canvas_update
};

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

	return self;
}

void Canvas_destructor(PObject self) {
	Widget_destructor(self);
}

PObject Canvas_clone(PObject self, PObject dst) {
	PTextBox real_self = (PTextBox)self;
	PTextBox real_dst  = (PTextBox)dst;

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


void Canvas_paint(PWidget self,WORD base_x, WORD base_y) {
	//GetDefaultWidgetRenderer()->vtable->paintCanvas(GetDefaultWidgetRenderer(), (PCanvas)self, base_x, base_y);
	ClippingRegion saved_region = ABSTRACTPAINTER(DEFAULTWIDGETRENDERER(self)->painter)->clipping_region;
	PListNode current_shape = NULL;

	/* Simply draw a rectangle */
	ABSTRACTPAINTER_VTABLE(self->painter)->drawRectangle(
			self->painter,
			//real_self->painter->abstract_painter.vtable->drawRectangle(
			//&real_self->painter->abstract_painter,
			base_x + self->x,
			base_y + self->y,
			self->width,
			self->height,
			RGB_16B(200,200,200),
			0
		);


	/* Test: define a clipping region for the painter */
	self->painter->clipping_region.x1 = base_x + self->x;
	self->painter->clipping_region.y1 = base_y + self->y;
	self->painter->clipping_region.x2 = base_x + self->x + self->width;
	self->painter->clipping_region.y2 = base_y + self->y + self->height;

	/* TODO: get each Shape of the model & paint it */
	if(CANVAS(self)->scene != NULL) {
		current_shape = CANVAS(self)->scene->shapes.head;
		
		while(current_shape != NULL) {
			PShape shape = SHAPEPTR(current_shape->data)->shape;

			shape->vtable->paint(shape, self->painter);

			current_shape = current_shape->next;
		}

	}

	self->painter->clipping_region = saved_region;
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