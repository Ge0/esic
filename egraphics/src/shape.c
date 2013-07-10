#include <esic/egraphics/shape.h>

PShape Shape_constructor(PShape self) {

	Vertice_constructor(&self->position);
	return self;
}

void Shape_destructor(PObject self) {
	Vertice_destructor(OBJECT(&SHAPE(self)->position));
}