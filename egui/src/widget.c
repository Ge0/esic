#include <esic/egui/widget.h>
#include <esic/eapi/event.h>

static const vtable_Object s_vtable_object = { NULL, NULL, NULL, NULL };

PWidget Widget_constructor(PWidget self) {
	self->object.size = sizeof(Widget);

	/* Filling vtable */
	self->object.vtable = &s_vtable_object;

	/* Initialize properties so they don't get dummy random values */
	self->x = self->y = self->width = self->height = self->color = 0;
	self->parent = NULL;

	/* Construct the list of childs */
	List_constructor(&self->childs, sizeof(PWidget));

	return self;
}

void Widget_addChild(PWidget self, PWidget child) {
	PPWidget child_to_add = &child;
	/* Set the parent, then add to the list */
	child->parent = self;
	List_pushBack(&self->childs.container, (PObject)child_to_add);
}

void Widget_defaultProc(PWidget self, PEvent systemEvent) {

}

