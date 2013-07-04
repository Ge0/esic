#include <esic/egraphics/rectangle.h>

VTABLE_START(Object) {
#define OBJECT_VFUNCTION(return_type, function_name, arguments) Rectangle_##function_name,
	OBJECT_VIRTUAL_FUNCTIONS
#undef OBJECT_VFUNCTION
};

VTABLE_START(Shape) {
#define SHAPE_VFUNCTION(return_type, function_name, arguments) Rectangle_##function_name,
	SHAPE_VIRTUAL_FUNCTIONS
#undef WIDGET_VFUNCTION
};

PRectangle Rectangle_constructor(PRectangle self) {
	/* TODO. */
	/* Filling vtables */
	OBJECT(self)->vtable = VTABLE_POINTER(Object);
	SHAPE(self)->vtable  = VTABLE_POINTER(Shape);

	return self;
}

void Rectangle_destructor(PObject self) {
	/* TODO. */
}

PObject Rectangle_clone(PObject self, PObject dst) {
	/* TODO. */
	return dst;
}

BOOL Rectangle_equalsTo(PObject self, PObject dst) {
	/* TODO. */
	return FALSE;
}

DWORD Rectangle_hash(PObject self) {
	/* TODO. */
	return 0;
}

void Rectangle_paint(PShape self, PPainter painter) {
	/*
	ABSTRACTPAINTER_VTABLE(painter)->drawRectangle(
		painter,
		RECTANGLE(self)->x,
		RECTANGLE(self)->y,
		RECTANGLE(self)->width,
		RECTANGLE(self)->height,
		self->color,
		self->border_color
	);
	*/
}
