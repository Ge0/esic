#include <esic/egui/graphicsview/graphics_scene.h>
#include <esic/egraphics/shape_ptr.h>


VTABLE_START(Object) {
	GraphicsScene_destructor,
	GraphicsScene_clone,
	GraphicsScene_equalsTo,
	GraphicsScene_hash,
	GraphicsScene_type
};

PGraphicsScene GraphicsScene_constructor(PGraphicsScene self) {

	Observable_constructor(OBSERVABLE(self));

	OBJECT(self)->size   = sizeof(GraphicsScene);
	OBJECT(self)->vtable = VTABLE_POINTER(Object);

	List_constructor(&self->shapes, sizeof(ShapePtr));

	return self;
}

void GraphicsScene_destructor(PObject self) {
	PListNode iterator = NULL;
	Observable_destructor(self);

	iterator = GRAPHICSSCENE(self)->shapes.head;

	while(iterator != NULL) {
		PShape current_shape = (PShape)((PShapePtr)iterator->data)->shape;
		if(((PShapePtr)iterator->data)->dynamic) {
			DELETE(current_shape);
		}
		//DELETE(WIDGETPTR(iterator->data)->widget);
		//WIDGETPTR(iterator->data)->widget = NULL;
		iterator = iterator->next;
	}

	List_destructor(OBJECT(&GRAPHICSSCENE(self)->shapes));
}

PObject GraphicsScene_clone(PObject self, PObject dst) {
	/* TODO. */

	return NULL;
}

BOOL GraphicsScene_equalsTo(PObject self, PObject dst) {
	/* TODO. */

	return FALSE;
}


DWORD GraphicsScene_hash(PObject self) {

	return 0;
}

DWORD GraphicsScene_type(PObject self) {
	static unsigned long s_hash = 0;

	if(s_hash == 0) {
		s_hash = Hash("GraphicsScene");
	}

	return s_hash;
}

void GraphicsScene_addShape(PGraphicsScene self, PShape shape, BOOL dynamic) {
	ShapePtr shape_ptr;
	ShapePtr_constructor(&shape_ptr, shape, dynamic);

	List_pushBack(CONTAINER(&self->shapes), OBJECT(&shape_ptr));

	ShapePtr_destructor(OBJECT(&shape_ptr));


	/* Assign the observable to 'changed' & notify the observers */
	self->observable.changed = TRUE;

	//Observable_notify(&self->observable, NULL);
}