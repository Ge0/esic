/**
 * \file widget.c
 */
#include <esic/eapi/system.h>
#include <esic/egui/widget.h>
#include <esic/egui/widget_ptr.h>
#include <esic/eapi/event.h>
#include <esic/egraphics/factories/painters_factory.h>

/*
static const vtable_Object s_vtable_object = {
	Widget_destructor,
	Widget_clone,
	NULL,
	NULL
};
*/

VTABLE_START(Object) {
	Widget_destructor,
	Widget_clone,
	Widget_equalsTo,
	Widget_hash,
	Widget_type
};

VTABLE_START(Widget) {
	Widget_defaultProc,
	Widget_paint,
	Widget_handleWidgetEvent
};

/*
static DWORD _handle_widget_event(PWidget self, PWidgetEvent widget_event);
*/

PWidget Widget_constructor(PWidget self) {

	self->object.size = sizeof(Widget);

	/* Filling vtable */
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	self->vtable         = VTABLE_POINTER(Widget);

	/* Initialize properties so they don't get dummy random values */
	self->x = self->y = self->width = self->height = self->color = 0;
	self->is_hot       = FALSE;
	self->parent       = NULL;
	self->is_activated = TRUE;
	self->is_visible   = TRUE;

	/* Construct the list of childs */
	List_constructor(&self->childs, sizeof(WidgetPtr));

	return self;
}


void Widget_destructor(PObject self) {
	/* For each childs, destruct it */
	//PWidget real_self = (PWidget)self;
	PListNode iterator = WIDGET(self)->childs.head;
	
	while(iterator != NULL) {
		PWidget current_child_widget = (PWidget)((PWidgetPtr)iterator->data)->widget;
		if(((PWidgetPtr)iterator->data)->dynamic) {
			DELETE(current_child_widget);
		}
		//DELETE(WIDGETPTR(iterator->data)->widget);
		//WIDGETPTR(iterator->data)->widget = NULL;
		iterator = iterator->next;
	}

	List_destructor(&WIDGET(self)->childs.container.object);
}

PObject Widget_clone(PObject self, PObject dst) {

	/* Copying members */
	dst->size   = self->size;
	dst->vtable = self->vtable;

	WIDGET(dst)->vtable       = WIDGET(self)->vtable;
	WIDGET(dst)->x            = WIDGET(self)->x;
	WIDGET(dst)->y            = WIDGET(self)->y;
	WIDGET(dst)->width        = WIDGET(self)->width;
	WIDGET(dst)->height       = WIDGET(self)->height;
	WIDGET(dst)->parent       = WIDGET(self)->parent;
	WIDGET(dst)->color        = WIDGET(self)->color;
	WIDGET(dst)->is_focusable = WIDGET(self)->is_focusable;
	WIDGET(dst)->is_hot       = WIDGET(self)->is_hot;


	/* clone the list of childs */
	OBJECT_VTABLE(self)->clone(
		OBJECT(self),
		OBJECT(dst)
	);

	/*
	real_self->childs.container.object.vtable->clone(
		&real_self->childs.container.object,
		&real_dst->childs.container.object
	);*/


	return dst;
}

DWORD Widget_type(PObject self) {
	static unsigned long s_hash = 0;
	if(s_hash == 0) {
		s_hash = Hash("Widget");
	}

	return s_hash;
}

void Widget_addChild(PWidget self, PWidget child, BOOL dynamic) {

	/* Instantiate a temp WidgetPtr instance...
	Since we store pointers, we still need to handle Object sub-classes! */
	WidgetPtr widgetptr;

	WidgetPtr_constructor(&widgetptr, child, dynamic);
	/* Set the parent, then add to the list */
	child->parent = self;
	//List_pushBack(&self->childs.container, (PObject)&widgetptr);
	List_pushBack(CONTAINER(&self->childs), OBJECT(&widgetptr));
	WidgetPtr_destructor(OBJECT(&widgetptr));
}

DWORD Widget_defaultProc(PWidget self, const PEvent system_event) {
	static PListNode hot_widget = NULL;
	PListNode it = NULL;
	PWidget current_child;
	switch(system_event->type) {
	case EVENT_WIDGET:

		WIDGET_VTABLE(self)->handleWidgetEvent(self, &system_event->real_event.widget_event);

		it = self->childs.head;

		/* No id? dispatch the event to every child */
		while(it != NULL) {
			PWidget chld = ((PWidgetPtr)it->data)->widget;
			/* If the event is to the current child, forward it. */
			if(system_event->real_event.widget_event.id == chld->id || system_event->real_event.widget_event.id == 0) {
				//WIDGET_VTABLE(chld)->defaultProc(WIDGET(chld), system_event);
				WIDGET_VTABLE(chld)->handleWidgetEvent(WIDGET(chld), &system_event->real_event.widget_event);
			}
			it = it->next;
		}
		break;

	case EVENT_TIMER:
		/* Forward to every childs */
		it = self->childs.head;

		while(it != NULL) {
			current_child = WIDGETPTR(hot_widget->data)->widget;
			WIDGET_VTABLE(current_child)->defaultProc(WIDGET(current_child), system_event);
			it = it->next;
		}
		break;

	}

	return 0;
}


void Widget_paint(PWidget self, PPainter painter, WORD base_x, WORD base_y) {
	/* Default painting procedure... Simply paint every childs */
	PListNode iterator = self->childs.head;

	while(iterator != NULL) {
		PWidget current_child = (PWidget)((PWidgetPtr)iterator->data)->widget;

		WIDGET_VTABLE(current_child)->paint(
			WIDGET(current_child),
			painter,
			base_x + self->x,
			base_y + self->y
		);

		iterator = iterator->next;
	}
}

BOOL Widget_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Widget_hash(PObject self) {
	return 0;
}

PWidget Widget_findChildById(PWidget self, WORD id) {
	PListNode iterator = self->childs.head;
	while(iterator != NULL && WIDGETPTR(iterator->data)->widget->id != id) {
		iterator = iterator->next;
	}

	if(iterator) {
		return WIDGETPTR(iterator->data)->widget;
	} else{
		return NULL;
	}
}

void Widget_handleWidgetEvent(PWidget self, PWidgetEvent widget_event) {
	DWORD base_x, base_y;
	PListNode current_painter = NULL;
	if(self->parent != NULL) {
		base_x = self->parent->x;
		base_y = self->parent->y;
	} else {
		base_x = 0;
		base_y = 0;
	}

	switch(widget_event->type) {
	case WE_PAINT:
			current_painter = GetPainters()->head;
			while(current_painter != NULL) {
				WIDGET_VTABLE(self)->paint(WIDGET(self), PAINTER(current_painter->data), base_x, base_y);
				current_painter = current_painter->next; 
			}
		break;
	}
}