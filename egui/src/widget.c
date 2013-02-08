#include <esic/egui/widget.h>
#include <esic/egui/widget_ptr.h>
#include <esic/eapi/event.h>

static const vtable_Object s_vtable_object = {
	Widget_destructor,
	Widget_clone,
	NULL,
	NULL
};

PWidget Widget_constructor(PWidget self) {
	self->object.size = sizeof(Widget);

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	/* Initialize properties so they don't get dummy random values */
	self->x = self->y = self->width = self->height = self->color = 0;
	self->parent = NULL;

	/* Construct the list of childs */
	List_constructor(&self->childs, sizeof(WidgetPtr));

	return self;
}


void Widget_destructor(PObject self) {
	/* For each childs, destruct it */
	PWidget real_self = (PWidget)self;
	PListNode iterator = real_self->childs.head;
	
	while(iterator != NULL) {
		PWidget current_child_widget = (PWidget)((PWidgetPtr)iterator->data)->widget;
		DELETE(current_child_widget);
		iterator = iterator->next;
	}

	List_destructor(&real_self->childs.container.object);
}

PObject Widget_clone(PObject self, PObject dst) {
	PWidget real_self = (PWidget)self;
	PWidget real_dst  = (PWidget)dst;

	/* Copying members */
	dst->size   = self->size;
	dst->vtable = self->vtable;

	real_dst->vtable = real_self->vtable;
	real_dst->x      = real_self->x;
	real_dst->y      = real_self->y;
	real_dst->width  = real_self->width;
	real_dst->height = real_self->height;
	real_dst->parent = real_self->parent;
	real_dst->color  = real_self->color;


	/* clone the list of childs */
	real_self->childs.container.object.vtable->clone(
		&real_self->childs.container.object,
		&real_dst->childs.container.object
	);


	return dst;
}

void Widget_addChild(PWidget self, PWidget child) {

	/* Instantiate a temp WidgetPtr instance...
	Since we store pointers, we still need to handle Object sub-classes! */
	WidgetPtr widgetptr;
	WidgetPtr_constructor(&widgetptr, child);
	/* Set the parent, then add to the list */
	child->parent = self;
	List_pushBack(&self->childs.container, (PObject)&widgetptr);

	WidgetPtr_destructor(&widgetptr.object);
}

void Widget_defaultProc(PWidget self, PEvent systemEvent) {

}

